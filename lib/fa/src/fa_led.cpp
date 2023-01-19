// Copyright (c) 2023 by Stefan Schmidt
#include "fa_led.h"
#include "fa_defines.h"
#include "fa_structs.h"
#include "fa_common.h"

extern fa_state_t state;

static uint32_t led_now = 0U;

void ledUpdate()
{
    static bool led_on = false;
    if (state.is_online)
    {
        if (!led_on)
        {
            led_on = true;
            led_enable(led_on);
        }
    }
    else if (intervalCheckMS(led_now, 100U))
    {
        led_on = !led_on;
        led_enable(led_on);
    }
}