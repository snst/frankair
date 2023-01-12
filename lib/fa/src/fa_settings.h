// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_SETTINGS_H
#define FA_SETTINGS_H

#include "fa_structs.h"
#include "fa_defines.h"



extern fa_settings_t fa_settings;

void calibration_load();
void calibration_write();
void calibration_clear();

void settings_load();
void settings_write();
void settings_clear();

void format();

#endif // FA_SETTINGS_H
