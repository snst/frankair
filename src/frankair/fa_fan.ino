// Copyright (c) 2022 by Stefan Schmidt
#include "fa_defines.h"

void fan_setup()
{
  ledcSetup(PWM1_CHANNEL, PWM_FREQ, PWM_RESULTION);
  ledcSetup(PWM2_CHANNEL, PWM_FREQ, PWM_RESULTION);

  ledcAttachPin(GPIO_PWM1, PWM1_CHANNEL);
  ledcAttachPin(GPIO_PWM2, PWM2_CHANNEL);
}

void fan_set(uint8_t channel, uint8_t pwm)
{
  ledcWrite(channel, 255 - pwm);
}