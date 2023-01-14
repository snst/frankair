// Copyright (c) 2023 by Stefan Schmidt
#include "fa_defines.h"
#include <Arduino.h>
#include "fa_led.h"

void ledSetup()
{
    pinMode(GPIO_LED, OUTPUT);
}

void led_enable(bool enable)
{
    digitalWrite(GPIO_LED, enable ? HIGH : LOW);
}
