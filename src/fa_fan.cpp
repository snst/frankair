// Copyright (c) 2022 by Stefan Schmidt
#include "fa_fan.h"

#include <pwmWrite.h>
#include "fa_log.h"

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
  val = val <= 10 ? 10 - val : 0;
  val = 255U * val / 10;
  pwm.write(gpio, val, 20U);
  IMSG("Gpio", gpio);
  IMSG("Set", val);
}

void fan_set_power_fresh(uint8_t val)
{
  fan_set(GPIO_PWM1, val);
}

void fan_set_power_exhaust(uint8_t val)
{
  fan_set(GPIO_PWM2, val);
}

void fan_set_power_frost(uint8_t val)
{
  fan_set(GPIO_PWM3, 10U - val);
}