// Copyright (c) 2023 by Stefan Schmidt
#include "fa_led.h"
#include "fa_defines.h"
#include "fa_structs.h"
#include "fa_common.h"
#include "fa_timer.h"

static FAInterval ledInterval;
static uint8_t led_blink_count = 0U;
bool led_on = false;

void ledUpdate()
{
    if (0U < led_blink_count)
    {
        if (ledInterval.checkMS(100U))
        {
            led_on = ledEnable(!led_on);
            led_blink_count--;
        }
    }
    else
    {
        if (state.errors > 0U)
        {
            if (ledInterval.checkMS(1000U))
            {
                led_on = ledEnable(!led_on);
            }
        }
        else if (state.is_online)
        {
            if (!led_on)
            {
                led_on = ledEnable(true);
            }
        }
        else if (ledInterval.checkMS(100U))
        {
            led_on = ledEnable(!led_on);
        }
    }
}

void ledBlink(uint8_t count)
{
    led_on = ledEnable(true);
    led_blink_count = count;
    ledInterval.reset();
}
