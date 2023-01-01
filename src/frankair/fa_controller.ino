// Copyright (c) 2022 by Stefan Schmidt
#include "fa_controller.h"
#include "fa_common.h"
#include "fa_fan.h"
#include "fa_servo.h"

static uint32_t controller_now = 0;
fa_state_t fa_state;

void controller_setup()
{
  fa_state.mode = controller_mode_t::kUndefined;
}

void controller_manual_update()
{
  //IMSG("MANUAL\n");
  if (update_if_changed(fa_state.actuator.power_fan_fresh, fa_settings.manual.power_fan_fresh, MANUAL_POWER_FAN_FRESH))
  {
    fan_set_power_fresh(fa_state.actuator.power_fan_fresh);
  }
  if (update_if_changed(fa_state.actuator.power_fan_exhaust, fa_settings.manual.power_fan_exhaust, MANUAL_POWER_FAN_EXHAUST))
  {
    fan_set_power_exhaust(fa_state.actuator.power_fan_exhaust);
  }
  if (update_if_changed(fa_state.actuator.power_fan_frost, fa_settings.manual.power_fan_frost, MANUAL_POWER_FAN_FROST))
  {
    fan_set_power_frost(fa_state.actuator.power_fan_frost);
  }
  if (update_if_changed(fa_state.actuator.flap_open_frost, fa_settings.manual.flap_open_frost, MANUAL_FLAP_OPEN_FROST))
  {
    servo_set(fa_state.actuator.flap_open_frost);
  }
}

void controller_auto_update()
{
  IMSG("AUTO\n");
}

void controller_off_update()
{
  IMSG("OFF\n");
}

void change_mode()
{
  // IMSG("Change mode to", fa_state.out.mode);
}

void controller_update()
{
  if (interval(controller_now, fa_settings.controller_interval_sec) || fa_settings.changed)
  {
    // IMSG("control!\n");
    fa_settings.changed = false;

    if (update_if_changed(fa_state.mode, fa_settings.mode, "Mode"))
    {
      change_mode();
    }

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
  }
}
