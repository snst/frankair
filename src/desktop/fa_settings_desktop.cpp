// Copyright (c) 2023 by Stefan Schmidt
#include "fa_settings.h"
#include "fa_log.h"

void settingsDefaultPlatform()
{
    settings.log_mask |= LSM;
    settings.sniff.sniff_sec = 5;
    settings.sniff.wait_sec = 10;
    settings.controller_interval_sec = 5;
    //override.enabled = true;
    state.is_online = true;
}

void settingsLoad()
{
    settingsDefault();
}

void settingsWrite()
{
}

void settingsClear()
{
}

void calibrationLoad()
{
}

void calibrationWrite()
{
}

void calibrationClear()
{
}

void format()
{
}
