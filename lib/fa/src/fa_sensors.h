// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_SENSORS_H
#define FA_SENSORS_H

#include "fa_common.h"

extern fa_override_t override;

void sensorsSetup();
void sensorsSetupPlatform();
void sensorsRead();
void sensorsUpdate();
void sensorsReadOverrideData();

static bool isOverrideEnabled()
{
    return override.enabled;
}

#endif // FA_SENSORS_H
