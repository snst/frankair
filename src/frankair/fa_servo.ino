// Copyright (c) 2022 by Stefan Schmidt
#include "fa_defines.h"
#include "fa_servo.h"
#include "fa_common.h"
#include <pwmWrite.h>

extern Pwm pwm;

void servo_setup()
{
    // servo_flap.attach(GPIO_SERVO);
    pwm.attach(GPIO_SERVO, 0U);
}

void servo_set(uint8_t val)
{
    pwm.writeServo(GPIO_SERVO, map(toRange(val, 0U, 10U), 0U, 10U, fa_settings.flap_min, fa_settings.flap_max));
}