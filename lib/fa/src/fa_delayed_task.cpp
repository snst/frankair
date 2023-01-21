// Copyright (c) 2023 by Stefan Schmidt
#include "fa_delayed_task.h"

static delayedTaskFunc func_ptr = 0U;
uint32_t delayed_ms = 0U;
uint32_t delayed_now = 0U;

void delayedTaskSetup()
{
}

void delayedTaskSet(delayedTaskFunc func, uint32_t ms)
{
    intervalReset(delayed_now);
    delayed_ms = ms;
    func_ptr = func;
}

void delayedTaskUpdate()
{
    if (func_ptr)
    {
        if (intervalCheckMS(delayed_now, delayed_ms))
        {
            IMSG(LINFO, "run delayed task");
            func_ptr();
        }
    }
}

void delayedTaskReboot()
{
    IMSG(LINFO, "delayedTaskReboot()");
    delayedTaskSet(rebootTarget, 5000U);
}