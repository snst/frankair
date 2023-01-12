// Copyright (c) 2023 by Stefan Schmidt
#include "fa_common.h"
#include "fa_settings.h"
#include "fa_controller.h"
#include "fa_fan.h"
#include "fa_flap.h"
#include "fa_log.h"

static uint32_t controller_now = 0;
fa_state_t fa_state;
fa_state_raw_t fa_state_raw;

void controller_setup()
{
  fa_state.mode = controller_mode_t::kUndefined;
}

void controller_manual_update()
{
  // IMSG("MANUAL\n");
  if (update_if_changed(fa_state.actuator.power_fan_fresh, fa_settings.manual.power_fan_fresh))
  {
    fan_set_power_fresh(fa_state.actuator.power_fan_fresh);
  }
  if (update_if_changed(fa_state.actuator.power_fan_exhaust, fa_settings.manual.power_fan_exhaust))
  {
    fan_set_power_exhaust(fa_state.actuator.power_fan_exhaust);
  }
  if (update_if_changed(fa_state.actuator.power_fan_frost, fa_settings.manual.power_fan_frost))
  {
    fan_set_power_frost(fa_state.actuator.power_fan_frost);
  }
  if (update_if_changed(fa_state.actuator.flap_open_frost, fa_settings.manual.flap_open_frost))
  {
    flap_set(fa_state.actuator.flap_open_frost);
  }
}

void updateFanLevel(uint8_t &dest, uint8_t val)
{
  if (val < dest)
  {
    dest = val;
  }
}

void calcFanLevelHumidity(uint8_t& fan_level)
{
  if (fa_settings.constraints.humidity_on)
  {
    // Stop fan if rel humidity in room is lower than minimum e.g. 50%
    if (fa_state.humidity.rel_exaust_in < fa_settings.constraints.humidity.rel_min_start)
    {
      updateFanLevel(fan_level, fa_settings.constraints.fan_level_min);
      IMSG(LM_FAN, "calcFanLevelHumidity, rel hum too low", fan_level);
    }

    // Stop fan if abs humidity delta is lower than minimum delta e.g. 0.5g/m³
    if (fa_state.humidity.abs_delta < fa_settings.constraints.humidity.abs_min_stop)
    {
      updateFanLevel(fan_level, fa_settings.constraints.fan_level_min);
      IMSG(LM_FAN, "calcFanLevelHumidity, abs humidity delta too low", fan_level);
    }

    // Don't start fan if currently off and abs humidity delta is lower than minimum delta e.g. 1g/m³
    if ((fa_state.actuator.power_fan_fresh == fa_settings.constraints.fan_level_min) && (fa_state.humidity.abs_delta < fa_settings.constraints.humidity.abs_min_start))
    {
      updateFanLevel(fan_level, fa_settings.constraints.fan_level_min);
      IMSG(LM_FAN, "calcFanLevelHumidity, fan currently off and abs humidity delta too low to start", fan_level);
    }
  }
}

void calcFanLevelHumidityAbs(uint8_t& fan_level)
{
  if (fa_settings.constraints.humidity_fan_on)
  {
    for (uint8_t i = 0U; i < MAX_FAN_CONSTRAINTS; i++)
    {
      if (fa_state.humidity.abs_delta < fa_settings.constraints.humidity_fan[i].val)
      {
        updateFanLevel(fan_level, fa_settings.constraints.humidity_fan[i].level);
        IMSG(LM_FAN, "calcFanLevelHumidityAbs", fan_level);
        return;
      }
    }
  }
}

void calcFanLevelTemp(uint8_t& fan_level)
{
  if (fa_settings.constraints.temp_fan_on)
  {
    for (uint8_t i = 0U; i < MAX_FAN_CONSTRAINTS; i++)
    {
      if (fa_state.temp.exhaust_in < fa_settings.constraints.temp_fan[i].val)
      {
        updateFanLevel(fan_level, fa_settings.constraints.temp_fan[i].level);
        IMSG(LM_FAN, "calcFanLevelTemp", fan_level);
        return;
      }
    }
  }
}

void controller_auto_update()
{
  IMSG(LM_COMMON, "AUTO");
  uint8_t fan_level = fa_settings.constraints.fan_level_max;
  calcFanLevelHumidity(fan_level);
  calcFanLevelHumidityAbs(fan_level);
  calcFanLevelTemp(fan_level);
  fan_set_power_fresh(fan_level);
  fan_set_power_exhaust(fan_level);
}

void controller_off_update()
{
  IMSG(LM_COMMON, "OFF");
}

void change_mode()
{
  // IMSG("Change mode to", fa_state.out.mode);
}

void controller_calculate()
{
  // fa_state.humidity.abs_delta = fa_state.humidity.abs_fresh_out - fa_state.humidity.abs_exaust_in;
}

void controller_update()
{
  if (interval(controller_now, fa_settings.controller_interval_sec) || g_force_update)
  {
    // IMSG("control!\n");

    if (update_if_changed(fa_state.mode, fa_settings.mode))
    {
      change_mode();
    }

    controller_calculate();

    switch (fa_state.mode)
    {
    case controller_mode_t::kAuto:
      controller_auto_update();
      break;
    case controller_mode_t::kManual:
      controller_manual_update();
      break;
    default:
    case controller_mode_t::kOff:
      controller_off_update();
      break;
    }

    g_force_update = false;
  }
}
