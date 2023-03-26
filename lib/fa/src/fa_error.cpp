// Copyright (c) 2023 by Stefan Schmidt
#include "fa_error.h"
#include "fa_structs.h"
#include "fa_thing.h"
#include "fa_sm.h"
#include <cstdio>

#define MAX_ERROR_STATE_COUNT 10U

uint8_t error_state_count = 0U;

void errorUpdate()
{
    if (0U < error_state_count)
    {
        error_state_count--;
        if (0U == error_state_count)
        {
            errorClear();
        }
        smSendEvent(fa_sm_gen_EventId::fa_sm_gen_EventId_EV_ERROR, settings.temp_sensor_read_interval_sec * 1000U);
        IMSG(LSM, __func__, error_state_count);
    }
    else
    {
        smSendEvent(fa_sm_gen_EventId::fa_sm_gen_EventId_EV_SETTINGS_CHANGED, 1000U);
    }
}

void errorSet(uint8_t error)
{
    error_state_count = MAX_ERROR_STATE_COUNT;

    if (!isError())
    {
        thingCallEndpoint("error");
    }

    state.errors |= error;

    if (!smInState(fa_sm_gen_StateId::fa_sm_gen_StateId_ERROR))
    {
        smSendEvent(fa_sm_gen_EventId::fa_sm_gen_EventId_EV_ERROR);
    }
}

void errorClear()
{
    error_state_count = 0U;
    state.errors = 0U;
    smSendEvent(fa_sm_gen_EventId::fa_sm_gen_EventId_EV_SETTINGS_CHANGED, 1000U);
}

bool isError()
{
    return state.errors > 0U;
}
