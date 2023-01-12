// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_SENSORS_H
#define FA_SENSORS_H

#include "fa_common.h"

extern bool s_flag_scan_sensors;

void sensors_setup();
void sensorsRead();
void sensors_update();
void sensors_scan();
void sensors_scan_intern();

#endif // FA_SENSORS_H
