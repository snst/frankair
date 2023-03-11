// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_THING_H
#define FA_THING_H

#include "fa_common.h"

void thingSetup();
void thingUpdate();
void thingCallEndpoint(const char* endpoint_name);
void thingStream();


#endif // FA_THING_H
