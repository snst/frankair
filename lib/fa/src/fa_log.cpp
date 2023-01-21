// Copyright (c) 2023 by Stefan Schmidt
#include "fa_log.h"
#include <cstdio>

fa_log_mask_t log_mask;

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

uint8_t getMask()
{
    uint8_t mask = 0U;
    mask |= log_mask.error ? LERROR : 0U;
    mask |= log_mask.info ? LINFO : 0U;
    mask |= log_mask.debug ? LDEBUG : 0U;
    mask |= log_mask.sensor ? LSENSOR : 0U;
    mask |= log_mask.controller ? LCONTROLLER : 0U;
    mask |= log_mask.iot ? LIOT : 0U;
    mask |= log_mask.actuator ? LACTUATOR : 0U;
    return mask;
}

void setMask(uint8_t mask)
{
    log_mask.error = mask & LERROR;
    log_mask.info = mask & LINFO;
    log_mask.debug = mask & LDEBUG;
    log_mask.sensor = mask & LSENSOR;
    log_mask.controller = mask & LCONTROLLER;
    log_mask.iot = mask & LIOT;
    log_mask.actuator = mask & LACTUATOR;
}

void logTempHumidity(uint8_t level, const char *info, fa_temp_t &temp,
                     fa_humidity_t &humidity)
{
    char buf[128];
    sprintf(buf, "%s ei %.1f, fo %.1f, fi %.1f, eo %.1f, hi %.1f, ho %.1f",
            info,
            temp.exhaust_in,
            temp.fresh_out,
            temp.fresh_in,
            temp.exhaust_out,
            humidity.rel_exhaust_in,
            humidity.rel_fresh_out);
    IMSG(level, buf);
}
