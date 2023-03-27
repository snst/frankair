// Copyright (c) 2023 by Stefan Schmidt
#include "fa_error.h"
#include "fa_structs.h"
#include "fa_thing.h"
#include "fa_sm.h"
#include <cstdio>

void errorSet(uint8_t error)
{
    if (!isError())
    {
        thingCallEndpoint("error");
        smSendEvent(fa_sm_gen_EventId::fa_sm_gen_EventId_EV_ERROR, 0U);
    }
    state.errors |= error;
}

void errorClear()
{
    state.errors = 0U;
    smSendEvent(fa_sm_gen_EventId::fa_sm_gen_EventId_EV_SETTINGS_CHANGED, 1000U);
}

bool isError()
{
    return state.errors > 0U;
}
