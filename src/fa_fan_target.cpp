// Copyright (c) 2023 by Stefan Schmidt
#include "fa_fan.h"
#include <Arduino.h>
#include <pwmWrite.h>
#include "fa_defines.h"

Pwm pwm = Pwm();

void fan_setup()
{
  pwm.attach(GPIO_PWM1, 2);
  pwm.attach(GPIO_PWM2, 3);
  pwm.attach(GPIO_PWM3, 4);
}

void fan_set(uint8_t gpio, uint8_t val)
{
  pwm.write(gpio, val, 20U);
}
