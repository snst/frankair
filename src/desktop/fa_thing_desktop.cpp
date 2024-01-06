// Copyright (c) 2023 by Stefan Schmidt
#include "fa_thing.h"
#include "fa_log.h"

void thingCallEndpoint(const char *endpoint_name)
{
}

void thingSendStreamState()
{
    IMSG(LSM, __func__, state.sm_state);
}

void thingUpdate()
{
}

void thingSetupPlatform()
{
}