// Copyright (c) 2023 by Stefan Schmidt
#include "fa_log.h"

const char *modeToStr(uint8_t mode)
{
    return modeToStr((controller_mode_t)mode);
}
const char *submodeToStr(uint8_t mode)
{
    return submodeToStr((controller_submode_auto_t)mode);
}

const char *modeToStr(controller_mode_t mode)
{
    switch (mode)
    {
    case controller_mode_t::kOff:
        return "kOff";
    case controller_mode_t::kManual:
        return "kManual";
    case controller_mode_t::kAuto:
        return "kAuto";
    case controller_mode_t::kUndefined:
        return "kUndefined";
    default:
        return "???";
    }
}

const char *submodeToStr(controller_submode_auto_t mode)
{
    switch (mode)
    {
    case controller_submode_auto_t::kWait:
        return "kWait";
    case controller_submode_auto_t::kOn:
        return "kOn";
    case controller_submode_auto_t::kSniff:
        return "kSniff";
    case controller_submode_auto_t::kUndefined:
        return "kUndefined";
    default:
        return "???";
    }
}