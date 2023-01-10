// Copyright (c) 2023 by Stefan Schmidt
#include "fa_defines.h"

#include "fa_led.h"
#include "fa_structs.h"

extern fa_state_t fa_state;

static uint32_t led_now = 0U;
static uint32_t blink_interval = 1000U;

void led_setup()
{
    pinMode(GPIO_LED, OUTPUT);
}

void led_enable(bool enable)
{
    digitalWrite(GPIO_LED, enable ? HIGH : LOW);
}

void led_update()
{
    static bool led_on = false;
    if (interval(led_now, blink_interval))
    {
        led_on = !led_on;
        led_enable(led_on);
    }

    blink_interval = fa_state.is_online ? 1000U : 100U;
}