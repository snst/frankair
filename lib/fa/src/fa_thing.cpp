// Copyright (c) 2023 by Stefan Schmidt
#include "fa_thing.h"
#include "fa_timer.h"
#include "fa_sm.h"

FAEvent<uint8_t> eventStream;

void thingSetup()
{
    thingSetupPlatform();
    eventStream.send(10000U);
}

void thingTriggerStreamUpdate()
{
    eventStream.send(1000U);
}

void thingUpdateStream()
{
    if (eventStream.update())
    {
        if ((g_state_machine.state_id != fa_sm_gen_StateId_SNIFF) && (g_state_machine.state_id != fa_sm_gen_StateId_WAIT))
        {
            thingSendStreamState();
        }
        eventStream.send(settings.stream_interval_min * 60U * 1000U);
    }
}