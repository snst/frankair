// Copyright (c) 2023 by Stefan Schmidt
#include "fa_timer.h"
#include <math.h>
#include <stdio.h>
#include <sys/time.h>

uint32_t gNow = 0U;

uint32_t now()
{
    return gNow;
}

void nowUpdate()
{
    gNow = getMillis();
}

FAInterval::FAInterval()
    : mMs(0U) {}

void FAInterval::reset()
{
    mMs = now();
}

uint32_t FAInterval::checkSec(uint32_t sec)
{
    return checkMS(1000U * sec);
}

uint32_t FAInterval::checkMS(uint32_t ms)
{
    uint32_t diff = now() - mMs;
    if (diff >= ms)
    {
        mMs = now();
        return diff;
    }
    return 0U;
}