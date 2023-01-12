// Copyright (c) 2023 by Stefan Schmidt
#include "fa_fan.h"
#include "fa_log.h"
#include "fa_calibration.h"
#include "fa_controller.h"

void fan_set_power_main(uint8_t gpio, uint8_t val)
{
  val = toRange(val, 0U, 10U);
  fan_set(gpio, 255U - fa_calibration_actuator.fan_power_main[val]);
}

void fan_set_power_fresh(uint8_t val)
{
  fan_set_power_main(GPIO_PWM1, val);
  fa_state.actuator.power_fan_fresh = val;
}

void fan_set_power_exhaust(uint8_t val)
{
  fan_set_power_main(GPIO_PWM2, val);
  fa_state.actuator.power_fan_exhaust = val;
}

void fan_set_power_frost(uint8_t val)
{
  val = toRange(val, 0U, 10U);
  fan_set(GPIO_PWM3, fa_calibration_actuator.fan_power_frost[val]);
  fa_state.actuator.power_fan_frost = val;
}