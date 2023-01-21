// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_SENSORS_H
#define FA_SENSORS_H

#include "fa_common.h"

extern fa_override_t fa_override_sensors;

void sensorsSetup();
void sensorsTargetSetup();
void sensorsRead();
void sensorsUpdate();
void sensorsReadOverrideData();

#endif // FA_SENSORS_H
