// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_SETTINGS_H
#define FA_SETTINGS_H

#include "fa_structs.h"
#include "fa_defines.h"

extern fa_settings_t settings;

void calibrationLoad();
void calibrationWrite();
void calibrationClear();
void calibrationDefault();

void settingsLoad();
void settingsWrite();
void settingsClear();
void settingsDefault();
void settingsDefaultPlatform();
void settingsSanitize();

void format();

#endif // FA_SETTINGS_H
