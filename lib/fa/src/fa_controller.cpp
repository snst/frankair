// Copyright (c) 2023 by Stefan Schmidt
#include "fa_common.h"
#include "fa_settings.h"
#include "fa_controller.h"
#include "fa_fan.h"
#include "fa_flap.h"
#include "fa_log.h"

static uint32_t controller_now = 0U;
static uint32_t sniff_now = 0U;
static uint32_t wait_now = 0U;
fa_state_t state;
fa_state_raw_t state_raw;

void controllerSetup()
{
  state.mode = controller_mode_t::kUndefined;
  state.submode_auto = controller_submode_auto_t::kUndefined;
}

void controllerModeManualUpdate()
{
  // IMSG("MANUAL\n");
  if (updateIfChanged(state.actuator.level_fan_fresh, settings.manual.level_fan_fresh))
  {
    fanSetLevelFresh(state.actuator.level_fan_fresh);
  }
  if (updateIfChanged(state.actuator.level_fan_exhaust, settings.manual.level_fan_exhaust))
  {
    fanSetLevelExhaust(state.actuator.level_fan_exhaust);
  }
  if (updateIfChanged(state.actuator.level_fan_frost, settings.manual.level_fan_frost))
  {
    fanSetLevelFrost(state.actuator.level_fan_frost);
  }
  if (updateIfChanged(state.actuator.open_flap_frost, settings.manual.open_flap_frost))
  {
    flapSetOpen(state.actuator.open_flap_frost);
  }
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

bool calcFanOffHumidity(uint8_t &fan_level)
{
  bool active = false;
  if (settings.ctrl.humidity_fan_off.enabled)
  {
    // Stop fan if rel humidity in room is lower than minimum e.g. 50%
    if (state.humidity.rel_exaust_in < settings.ctrl.humidity_fan_off.rel_min_start)
    {
      active = reduceFanLevel(fan_level, settings.ctrl.fan_level_min) || active;
      IMSG(LM_FAN, "calcFanOffHumidity: Relative humidity too low. Fan", fan_level);
    }

    switch (state.submode_auto)
    {
    case controller_submode_auto_t::kOn:
      // Stop fan if abs humidity delta is lower than minimum delta e.g. 0.5g/m³
      if (state.humidity.abs_delta < settings.ctrl.humidity_fan_off.abs_min_stop)
      {
        active = reduceFanLevel(fan_level, settings.ctrl.fan_level_min) || active;
        IMSG(LM_FAN, "calcFanOffHumidity: Absolute humidity delta too low. Fan", fan_level);
      }
      break;
    case controller_submode_auto_t::kSniff:
      // Don't start fan if currently sniffing and abs humidity delta is lower than minimum delta e.g. 1g/m³
      if (state.humidity.abs_delta < settings.ctrl.humidity_fan_off.abs_min_start)
      {
        active = reduceFanLevel(fan_level, settings.ctrl.fan_level_min) || active;
        IMSG(LM_FAN, "calcFanOffHumidity: Currently sniffing and absolute humidity delta too low to start. Fan", fan_level);
      }
      break;
    }
  }
  state.ctrl_active.humidity_fan_off = active;
  return active;
}

bool calcFanLevelHumidityAbs(uint8_t &fan_level)
{
  bool active = false;
  if (settings.ctrl.humidity_fan_level.enabled)
  {
    for (uint8_t i = 0U; i < MAX_FAN_CONSTRAINTS; i++)
    {
      if (state.humidity.abs_delta < settings.ctrl.humidity_fan_level.item[i].val)
      {
        active = reduceFanLevel(fan_level, settings.ctrl.humidity_fan_level.item[i].level) | active;
        IMSG(LM_FAN, "calcFanLevelHumidityAbs. Fan", fan_level);
      }
    }
  }
  state.ctrl_active.humidity_fan_level = active;
  return active;
}

bool calcFanLevelTemp(uint8_t &fan_level)
{
  bool active = false;
  if (settings.ctrl.temp_fan_level.enabled)
  {
    for (uint8_t i = 0U; i < MAX_FAN_CONSTRAINTS; i++)
    {
      if (state.temp.exhaust_in < settings.ctrl.temp_fan_level.item[i].val)
      {
        active = reduceFanLevel(fan_level, settings.ctrl.temp_fan_level.item[i].level) | active;
        IMSG(LM_FAN, "calcFanLevelTemp. Fan", fan_level);
      }
    }
  }
  state.ctrl_active.temp_fan_level = active;
  return active;
}

void controllerModeAutoChangeSubMode(controller_submode_auto_t submode)
{
  controller_submode_auto_t old = state.submode_auto;
  if (updateIfChanged(state.submode_auto, submode))
  {
    IMSG(LM_MODE, "controllerModeAutoChangeSubMode: ", submodeToStr(old), " => ", submodeToStr(submode));
  }
}

void controllerStartSniff()
{
  IMSG(LM_MODE, "controllerStartSniff()");
  controllerModeAutoChangeSubMode(controller_submode_auto_t::kSniff);
  fanSetLevelFreshAndExhaust(settings.sniff.fan_level);
  intervalReset(sniff_now);
}

void controllerStartWait()
{
  IMSG(LM_MODE, "controllerStartWait()");
  controllerModeAutoChangeSubMode(controller_submode_auto_t::kWait);
  fanSetLevelFreshAndExhaust(settings.ctrl.fan_level_min);
  intervalReset(wait_now);
}

void controllerModeAutoUpdateFan()
{
  IMSG(LM_MODE, "controllerModeAutoUpdateFan()");
  uint8_t fan_level = settings.ctrl.fan_level_max;
  if (calcFanOffHumidity(fan_level))
  {
    controllerStartWait();
  }
  else
  {
    calcFanLevelHumidityAbs(fan_level);
    calcFanLevelTemp(fan_level);
    if (FAN_LEVEL_MIN == fan_level)
    {
      controllerStartWait();
    }
    else
    {
      controllerModeAutoChangeSubMode(controller_submode_auto_t::kOn);
      fanSetLevelFreshAndExhaust(fan_level);
    }
  }
}

void controllerModeAutoUpdate()
{
  IMSG(LM_MODE, "controllerModeAutoUpdate() Submode: ", submodeToStr(state.submode_auto));

  switch (state.submode_auto)
  {
  case controller_submode_auto_t::kWait:
    if (intervalCheckSec(wait_now, settings.sniff.interval_sec))
    {
      IMSG(LM_MODE, "kWait finished => controllerStartSniff()");
      controllerStartSniff();
    }
    break;
  case controller_submode_auto_t::kOn:
    controllerModeAutoUpdateFan();
    break;
  case controller_submode_auto_t::kSniff:
    if (intervalCheckSec(sniff_now, settings.sniff.duration_sec))
    {
      IMSG(LM_MODE, "kSniff finished => controllerModeAutoUpdateFan()");
      controllerModeAutoUpdateFan();
    }
    break;
  default:
  case controller_submode_auto_t::kUndefined:
    IMSG(LM_MODE, "kUndefined => controllerStartSniff()");
    controllerStartSniff();
    break;
  }
}

void controllerModeChanged()
{
  IMSG(LM_MODE, "controllerMode changed to: ", modeToStr(state.mode));
  state.submode_auto = controller_submode_auto_t::kUndefined;
  switch (state.mode)
  {
  case controller_mode_t::kOff:
    fanSetLevelFreshAndExhaust(0U);
    fanSetLevelFrost(0U);
    flapSetOpen(0U);
    break;
  }
}

void controllerUpdate()
{
  if (intervalCheckSec(controller_now, settings.controller_interval_sec) || force_update)
  {
    if (updateIfChanged(state.mode, settings.mode))
    {
      controllerModeChanged();
    }

    switch (state.mode)
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

    force_update = false;
  }
}
