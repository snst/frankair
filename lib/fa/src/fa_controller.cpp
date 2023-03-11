// Copyright (c) 2023 by Stefan Schmidt
#include "fa_common.h"
#include "fa_settings.h"
#include "fa_controller.h"
#include "fa_fan.h"
#include "fa_flap.h"
#include "fa_log.h"
#include "fa_statistic.h"
#include "fa_error.h"
#include "fa_thing.h"
#include <math.h>

static uint32_t controller_now = 0U;
static uint32_t sniff_now = 0U;
static uint32_t wait_now = 0U;
static uint32_t stream_now = 0U;

fa_state_t state;
fa_state_raw_t state_raw;
bool force_update_controller = false;
bool update_stream = false;
extern fa_override_t override;

void controllerSetup()
{
  state.mode = (uint8_t)controller_mode_t::kUndefined;
  state.submode_auto = (uint8_t)controller_submode_auto_t::kUndefined;
}

void controllerModeManualUpdate()
{
  fanSetLevelFresh(settings.manual.level_fan_fresh);
  fanSetLevelExhaust(settings.manual.level_fan_exhaust);
  fanSetLevelFrost(settings.manual.level_fan_frost);
  flapSetOpen(settings.manual.open_flap_frost);
}

bool reduceFanLevel(uint8_t &dest, uint8_t val)
{
  if (val < dest)
  {
    dest = val;
    return true;
  }
  return false;
}

bool increaseFanLevel(uint8_t &dest, uint8_t val)
{
  if (val > dest)
  {
    dest = val;
    return true;
  }
  return false;
}

bool updateFanLevel(uint8_t &dest, uint8_t val, bool reduce)
{
  if (reduce)
  {
    return reduceFanLevel(dest, val);
  }
  else
  {
    return increaseFanLevel(dest, val);
  }
}

bool calcFanOffHumidity(uint8_t &fan_level)
{
  uint8_t off = 0U;
  if (settings.ctrl.humidity_fan_ctrl.enabled)
  {
    // Stop fan if rel humidity in room is lower than minimum e.g. 50%
    if (state.humidity.rel_exhaust_in <= settings.ctrl.humidity_fan_ctrl.rel_min_off)
    {
      reduceFanLevel(fan_level, settings.ctrl.fan_level_min);
      IMSG(LCONTROLLER, "Relative humidity too low. Set fan minimum. Hum", settings.ctrl.humidity_fan_ctrl.rel_min_off);
      off |= 1U;
    }
    else
    {
      switch ((controller_submode_auto_t)state.submode_auto)
      {
      case controller_submode_auto_t::kOn:
        // Stop fan if abs humidity delta is lower than minimum delta e.g. 0.5g/m³
        if (state.humidity.abs_delta <= settings.ctrl.humidity_fan_ctrl.abs_min_off)
        {
          reduceFanLevel(fan_level, settings.ctrl.fan_level_min);
          IMSG(LCONTROLLER, "kOn: Absolute humidity delta too low. Set fan minimum. Hum", settings.ctrl.humidity_fan_ctrl.abs_min_off);
          off |= 2U;
        }
        break;
      case controller_submode_auto_t::kSniff:
        // Don't start fan if currently sniffing and abs humidity delta is lower than minimum delta e.g. 1g/m³
        if (state.humidity.abs_delta < settings.ctrl.humidity_fan_ctrl.abs_min_on)
        {
          reduceFanLevel(fan_level, settings.ctrl.fan_level_min);
          IMSG(LCONTROLLER, "kSniff: Absolute humidity delta too low to start Fan. Hum", settings.ctrl.humidity_fan_ctrl.abs_min_on);
          off |= 4U;
        }
        break;
      }
    }
  }
  state.ctrl_active.humidity_fan_off = off;
  return 0U != off;
}

bool calcFanLevelCurve(float input, fa_fan_curve_t &curve, uint8_t &level)
{
  uint8_t n = curve.enabled_points;
  if (!curve.enabled || (n < 2U))
  {
    return false;
  }
  for (uint8_t i = 1U; i < n; i++)
  {
    if (isInRange(input, curve.item[i - 1U].val, curve.item[i].val))
    {
      float levelf = mapValue(input,
                              curve.item[i - 1U].val,
                              curve.item[i].val,
                              curve.item[i - 1U].level,
                              curve.item[i].level);
      level = roundf(levelf);
      return true;
    }
  }
  if (curve.item[0].val < curve.item[n - 1U].val)
  {
    if (input < curve.item[0].val)
    {
      level = curve.item[0].level;
    }
    else
    {
      level = curve.item[n - 1U].level;
    }
  }
  else
  {
    if (input < curve.item[n - 1U].val)
    {
      level = curve.item[n - 1U].level;
    }
    else
    {
      level = curve.item[0].level;
    }
  }
  return true;
}

bool calcFanLevelByHumidityCurve(uint8_t &fan_level)
{
  uint8_t new_level = fan_level;
  calcFanLevelCurve(state.humidity.abs_delta, settings.ctrl.humidity_fan_curve, new_level);
  bool active = reduceFanLevel(fan_level, new_level);
  state.ctrl_active.humidity_fan_curve = active ? new_level : settings.ctrl.fan_level_max;
  return active;
}

bool calcFanLevelByTempCurve(uint8_t &fan_level)
{
  uint8_t new_level = fan_level;
  calcFanLevelCurve(state.temp.exhaust_in, settings.ctrl.temp_fan_curve, new_level);
  bool active = reduceFanLevel(fan_level, new_level);
  state.ctrl_active.temp_fan_curve = active ? new_level : settings.ctrl.fan_level_max;
  return active;
}

uint8_t calcFanFrostLevel()
{
  uint8_t fan_level = FAN_LEVEL_OFF;
  if (state.actuator.open_flap_frost > FLAP_LEVEL_CLOSE)
  {
    uint8_t new_level = FAN_LEVEL_OFF;
    calcFanLevelCurve(state.temp.fresh_in, settings.ctrl.frost_fan_curve, new_level);
    bool active = increaseFanLevel(fan_level, new_level);
    state.ctrl_active.frost_fan_curve = active ? new_level : settings.ctrl.fan_frost_level_min;
  }
  return fan_level;
}

void controllerModeAutoChangeSubMode(controller_submode_auto_t submode)
{
  uint8_t old = state.submode_auto;
  if (updateIfChanged(state.submode_auto, (uint8_t)submode, update_stream))
  {
    IMSG(LCONTROLLER, "Submode: ", submodeToStr(old), " => ", submodeToStr(submode));
  }
}

void controllerForceUpdate()
{
  force_update_controller = true;
}

void controllerStartSniffing()
{
  IMSG(LDEBUG, "controllerStartSniffing()");
  controllerModeAutoChangeSubMode(controller_submode_auto_t::kSniff);
  intervalReset(sniff_now);
  controllerForceUpdate();
}

void controllerStartWait()
{
  IMSG(LDEBUG, "controllerStartWait()");
  controllerModeAutoChangeSubMode(controller_submode_auto_t::kWait);
  intervalReset(wait_now);
  controllerForceUpdate();
}

void controllerModeAutoUpdateFrost()
{
  IMSG(LDEBUG, "controllerModeAutoUpdateFrost()");

  if (controller_submode_auto_t::kWait == (controller_submode_auto_t)state.submode_auto)
  {
    // close flap if waiting
    flapClose();
    fanSetFrostOff();
  }
  else
  {
    if (settings.ctrl.frost_flap_ctrl.enabled)
    {
      float temp = state.temp.fresh_in;
      if (temp <= settings.ctrl.frost_flap_ctrl.temp_min_open)
      {
        flapOpen();
        IMSG(LCONTROLLER, "Temp fresh IN lower => open flap. Temp", settings.ctrl.frost_flap_ctrl.temp_min_open);
      }
      if (temp >= settings.ctrl.frost_flap_ctrl.temp_min_close)
      {
        flapClose();
        IMSG(LCONTROLLER, "Temp fresh IN higher => close flap. Temp", settings.ctrl.frost_flap_ctrl.temp_min_close);
      }

      fanSetLevelFrost(calcFanFrostLevel());
    }
  }
}

void controllerModeAutoOn()
{
  IMSG(LDEBUG, "controllerModeAutoOn()");
  logTempHumidity(LCONTROLLER, "", state.temp, state.humidity);

  uint8_t fan_level = settings.ctrl.fan_level_max;

  if (calcFanOffHumidity(fan_level))
  {
    controllerStartWait();
  }
  else
  {
    calcFanLevelByHumidityCurve(fan_level);
    calcFanLevelByTempCurve(fan_level);
    if (FAN_LEVEL_OFF == fan_level)
    {
      IMSG(LCONTROLLER, "Fan completely reduced => kWait");
      controllerStartWait();
    }
    else
    {
      // Fans are running
      controllerModeAutoChangeSubMode(controller_submode_auto_t::kOn);
      fanSetLevelMainWithOffset(fan_level);
      // fanSetLevelFrost(calcFanFrostLevel());
    }
  }
}

void controllerModeAutoUpdate()
{
  IMSG(LDEBUG, "controllerModeAutoUpdate() ", submodeToStr(state.submode_auto));

  switch ((controller_submode_auto_t)state.submode_auto)
  {
  case controller_submode_auto_t::kWait:
    if (intervalCheckSec(wait_now, settings.sniff.interval_sec))
    {
      IMSG(LDEBUG, "kWait finished => controllerStartSniffing()");
      controllerStartSniffing();
    }
    else
    {
      fanSetMainMinimum();
    }
    break;
  case controller_submode_auto_t::kOn:
    controllerModeAutoOn();
    break;
  default:
  case controller_submode_auto_t::kSniff:

    if (intervalCheckSec(sniff_now, settings.sniff.duration_sec))
    {
      IMSG(LDEBUG, "kSniff finished => controllerModeAutoOn()");
      controllerModeAutoOn();
    }
    else
    {
      fanSetMainSniffing();
    }
    break;
  }

  controllerModeAutoUpdateFrost();
}

void controllerModeChanged()
{
  IMSG(LCONTROLLER, "controllerMode changed to: ", modeToStr(state.mode));
  state.submode_auto = (uint8_t)controller_submode_auto_t::kUndefined;
  switch ((controller_mode_t)state.mode)
  {
  case controller_mode_t::kOff:
    fanSetMainOff();
    fanSetFrostOff();
    flapClose();
    break;
  }
}

void controllerUpdate()
{
  update_stream = false;
  uint32_t controller_ms = intervalCheckSec(controller_now, settings.controller_interval_sec);
  if ((0U < controller_ms) || force_update_controller)
  {
    force_update_controller = false;
    errorUpdate();
    statisticUpdate(controller_ms);

    if (updateIfChanged(state.mode, settings.mode, update_stream))
    {
      controllerModeChanged();
    }

    if ((0U < state.errors) && !override.enabled)
    {
      fanSetFrostOff();
      fanSetMainOff();
    }
    else
    {
      switch ((controller_mode_t)state.mode)
      {
      case controller_mode_t::kAuto:
        controllerModeAutoUpdate();
        break;
      case controller_mode_t::kManual:
        controllerModeManualUpdate();
        break;
      case controller_mode_t::kOff:
      default:
        break;
      }
    }
  }

  static uint32_t last_update_stream = 0U;
  if (update_stream || intervalCheckSec(stream_now, 600U))
  {
    if ((now() - last_update_stream) >= 2000U) // only stream if no updated within last 2 sec
    {
      update_stream = false;
      last_update_stream = now();
      intervalReset(stream_now);
      thingStream();
    }
  }
}

void controller_now_reset_for_test()
{
  controller_now = 0U;
}