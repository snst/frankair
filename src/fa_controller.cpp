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

void controller_auto_update()
{
  IMSG(LM_COMMON, "AUTO");
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
