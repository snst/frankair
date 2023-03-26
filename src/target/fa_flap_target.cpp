// Copyright (c) 2023 by Stefan Schmidt
#include "fa_defines.h"

#include <pwmWrite.h>

#include "fa_flap.h"
#include "fa_settings.h"
#include "fa_calibration.h"
#include "fa_common.h"
#include "fa_controller.h"

extern Pwm pwm;

void flapSetup()
{
    pwm.attach(GPIO_SERVO, 0U);
}

void flapSetOpenPWM(uint8_t val)
{
    pwm.writeServo(GPIO_SERVO, val);
}
