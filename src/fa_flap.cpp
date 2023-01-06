// Copyright (c) 2022 by Stefan Schmidt
#include "fa_defines.h"

#include <pwmWrite.h>

#include "fa_flap.h"
#include "fa_settings.h"

extern Pwm pwm;

void flap_setup()
{
    pwm.attach(GPIO_SERVO, 0U);
}

void flap_set(uint8_t val)
{
    pwm.writeServo(GPIO_SERVO, map(toRange(val, 0U, 10U), 0U, 10U, fa_settings.flap_min, fa_settings.flap_max));
}