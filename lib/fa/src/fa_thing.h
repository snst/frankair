// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_THING_H
#define FA_THING_H

#include "fa_common.h"

void thingSetup();
void thingSetupPlatform();
void thingUpdate();
void thingCallEndpoint(const char* endpoint_name);
void thingUpdateStream();
void thingTriggerStreamUpdate();
void thingSendStreamState();


#endif // FA_THING_H
