// Copyright (c) 2022 by Stefan Schmidt
#include "fa_defines.h"
#include "fa_servo.h"
#include <pwmWrite.h>

extern Pwm pwm;

void servo_setup()
{
    // servo_flap.attach(GPIO_SERVO);
}

void servo_set(uint8_t val)
{
    pwm.writeServo(GPIO_SERVO, 180 * val / 10);
}