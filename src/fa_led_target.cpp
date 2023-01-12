// Copyright (c) 2023 by Stefan Schmidt
#include "fa_defines.h"
#include <Arduino.h>
#include "fa_led.h"

void led_setup()
{
    pinMode(GPIO_LED, OUTPUT);
}

void led_enable(bool enable)
{
    digitalWrite(GPIO_LED, enable ? HIGH : LOW);
}
