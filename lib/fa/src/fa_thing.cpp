// Copyright (c) 2023 by Stefan Schmidt
#include "fa_thing.h"
#include "fa_timer.h"

FAEvent<uint8_t> eventStream;

void thingSetup()
{
    thingSetupPlatform();
    eventStream.send(10000U);
}

void thingTriggerStreamUpdate()
{
    eventStream.send(2000U);
}

void thingUpdateStream()
{
    if (eventStream.update())
    {
        thingSendStreamState();
        eventStream.send(settings.stream_interval_min * 60U * 1000U);
    }
}