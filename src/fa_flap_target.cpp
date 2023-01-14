// Copyright (c) 2023 by Stefan Schmidt
#include "fa_defines.h"

#include <pwmWrite.h>

#include "fa_flap.h"
#include "fa_settings.h"
#include "fa_calibration.h"

extern Pwm pwm;

void flapSetup()
{
    pwm.attach(GPIO_SERVO, 0U);
}

void flapSetOpen(uint8_t val)
{
    pwm.writeServo(GPIO_SERVO, map(toRange(val, 0U, 10U), 0U, 10U, fa_calibration_actuator.flap_pos.min, fa_calibration_actuator.flap_pos.max));
}
