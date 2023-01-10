// Copyright (c) 2023 by Stefan Schmidt
#include "fa_fan.h"

#include <pwmWrite.h>
#include "fa_log.h"
#include "fa_calibration.h"

Pwm pwm = Pwm();

void fan_setup()
{
  /*
  ledcSetup(PWM1_CHANNEL, PWM_FREQ, PWM_RESULTION);
  ledcSetup(PWM2_CHANNEL, PWM_FREQ, PWM_RESULTION);
  ledcSetup(PWM3_CHANNEL, PWM_FREQ, PWM_RESULTION);

  ledcAttachPin(GPIO_PWM1, PWM1_CHANNEL);
  ledcAttachPin(GPIO_PWM2, PWM2_CHANNEL);
  ledcAttachPin(GPIO_PWM3, PWM3_CHANNEL);*/
  pwm.attach(GPIO_PWM1, 2);
  pwm.attach(GPIO_PWM2, 3);
  pwm.attach(GPIO_PWM3, 4);
}

void fan_set(uint8_t gpio, uint8_t val)
{
  pwm.write(gpio, val, 20U);
  //  IMSG("Gpio", gpio);
  //  IMSG("Set", val);
}

void fan_set_power_main(uint8_t gpio, uint8_t val)
{
  val = toRange(val, 0U, 10U);
  fan_set(gpio, 255U - fa_calibration_actuator.fan_power_main[val]);
}

void fan_set_power_fresh(uint8_t val)
{
  fan_set_power_main(GPIO_PWM1, val);
}

void fan_set_power_exhaust(uint8_t val)
{
  fan_set_power_main(GPIO_PWM2, val);
}

void fan_set_power_frost(uint8_t val)
{
  val = toRange(val, 0U, 10U);
  fan_set(GPIO_PWM3, fa_calibration_actuator.fan_power_frost[val]);
}