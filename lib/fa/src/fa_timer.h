// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_TIMER_H
#define FA_TIMER_H

#include <stdint.h>
#include "fa_common.h"

extern uint32_t gNow;

uint32_t getMillis();
void nowUpdate();
uint32_t now();

template <typename T>
class FAEvent
{
public:
    FAEvent() : enabled(false) {}
    bool update(T &id)
    {
        if (enabled)
        {
            if (now() >= mExpireMs)
            {
                enabled = false;
                id = mId;
                return true;
            }
        }
        return false;
    }

    bool update()
    {
        if (enabled)
        {
            if (now() >= mExpireMs)
            {
                enabled = false;
                return true;
            }
        }
        return false;
    }

    void send(T id, uint32_t ms)
    {
        enabled = true;
        mId = id;
        mExpireMs = now() + ms;
    }

    void send(uint32_t ms)
    {
        enabled = true;
        mExpireMs = now() + ms;
    }

protected:
    T mId;
    uint32_t mExpireMs;
    bool enabled;
};

class FAInterval
{
public:
    FAInterval();
    void reset();
    uint32_t checkSec(uint32_t sec);
    uint32_t checkMS(uint32_t ms);

protected:
    uint32_t mMs;
};

#endif // FA_TIMER_H
