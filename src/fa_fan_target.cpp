// Copyright (c) 2023 by Stefan Schmidt
#include "fa_fan.h"
#include <Arduino.h>
#include <pwmWrite.h>
#include "fa_defines.h"

Pwm pwm = Pwm();

void fanSetup()
{
  pwm.attach(GPIO_PWM1, 2U);
  pwm.attach(GPIO_PWM2, 3U);
  pwm.attach(GPIO_PWM3, 4U);
}

void fanSetPWM(uint8_t gpio, uint8_t val)
{
  pwm.write(gpio, val, 20U);
}
