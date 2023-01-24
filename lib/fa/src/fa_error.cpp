// Copyright (c) 2023 by Stefan Schmidt
#include "fa_error.h"
#include "fa_structs.h"
#include "fa_thing.h"
#include <cstdio>

#define MAX_ERROR_STATE_COUNT 100U

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
    }
}

void errorSet(uint8_t error)
{
    uint8_t new_errors = state.errors | error;
    if (state.errors != new_errors)
    {
        state.errors = new_errors;

        thingCallEndpoint("error");
    }

    error_state_count = MAX_ERROR_STATE_COUNT;
}

void errorClear()
{
    error_state_count = 0U;
    state.errors = 0U;
}
