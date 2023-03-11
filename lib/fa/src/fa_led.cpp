// Copyright (c) 2023 by Stefan Schmidt
#include "fa_led.h"
#include "fa_defines.h"
#include "fa_structs.h"
#include "fa_common.h"

static uint8_t led_blink_count = 0U;
static bool led_on = false;
static uint32_t led_now = 0U;

void ledUpdate()
{
    if (0U < led_blink_count)
    {
        if (intervalCheckMS(led_now, 100U))
        {
            led_on = ledEnable(!led_on);
            led_blink_count--;
        }
    }
    else
    {
        if (state.is_online)
        {
            if (!led_on)
            {
                led_on = ledEnable(true);
            }
        }
        else if (intervalCheckMS(led_now, 100U))
        {
            led_on = ledEnable(!led_on);
        }
    }
}

void ledBlink(uint8_t count)
{
    led_on = ledEnable(true);
    led_blink_count = count;
    intervalReset(led_now);
}
