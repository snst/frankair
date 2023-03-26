// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_SM_DECISIONS_H
#define FA_SM_DECISIONS_H

#include "fa_structs.h"
#include "fa_sm.h"

#define FC_isFanLevelReduced 1U
#define FC_isRoomRelHumidityBelowMinimum 2U
#define FC_isAbsHumidityDeltaTooLowToStart 3U
#define FC_isAbsHumidityDeltaTooLowToRun 4U
#define FC_isRoomTempBelowDesiredMinTempAndFreshTempBelowRoomTemp 5U
#define FC_isRoomTempBelowDesiredMaxTempAndFreshTempOverRoomTemp 6U
#define FC_isFanLevelReducedToOff 7U

inline bool isRoomRelHumidityBelowMinimum()
{
    bool ret = settings.ctrl.simple.rel_min_enabled && (state.humidity.rel_exhaust_in <= settings.ctrl.simple.rel_min_wait);
    if (ret)
    {
        state.ctrl_active.decisions = FC_isRoomRelHumidityBelowMinimum;
        IMSG(LSM, __func__);
    }
    return ret;
}

inline bool isAbsHumidityDeltaTooLowToStart()
{
    bool ret = settings.ctrl.simple.abs_min_enabled && (state.humidity.abs_delta < settings.ctrl.simple.abs_hum_min_on);
    if (ret)
    {
        state.ctrl_active.decisions = FC_isAbsHumidityDeltaTooLowToStart;
        IMSG(LSM, __func__);
    }
    return ret;
}

inline bool isAbsHumidityDeltaTooLowToRun()
{
    bool ret = settings.ctrl.simple.abs_min_enabled && (state.humidity.abs_delta <= settings.ctrl.simple.abs_hum_min_wait);
    if (ret)
    {
        state.ctrl_active.decisions = FC_isAbsHumidityDeltaTooLowToRun;
        IMSG(LSM, __func__);
    }
    return ret;
}

inline bool isRoomTempBelowDesiredMinTempAndFreshTempBelowRoomTemp()
{
    bool ret = settings.ctrl.simple.desired_temp_min_enabled && (state.temp.exhaust_in <= settings.ctrl.simple.desired_temp_min) && (state.temp.fresh_out <= state.temp.exhaust_in);
    if (ret)
    {
        state.ctrl_active.decisions = FC_isRoomTempBelowDesiredMinTempAndFreshTempBelowRoomTemp;
        IMSG(LSM, __func__);
    }
    return ret;
}

inline bool isRoomTempBelowDesiredMaxTempAndFreshTempOverRoomTemp()
{
    bool ret = settings.ctrl.simple.desired_temp_max_enabled && (state.temp.exhaust_in <= settings.ctrl.simple.desired_temp_max) && (state.temp.fresh_out > state.temp.exhaust_in);
    if (ret)
    {
        state.ctrl_active.decisions = FC_isRoomTempBelowDesiredMaxTempAndFreshTempOverRoomTemp;
        IMSG(LSM, __func__);
    }
    return ret;
}

inline bool isFanLevelReducedToOff()
{
    bool ret = (state.actuator.level_fan_fresh <= FAN_LEVEL_OFF) || (state.actuator.level_fan_exhaust <= FAN_LEVEL_OFF);
    if (ret)
    {
        state.ctrl_active.decisions = FC_isFanLevelReducedToOff;
        IMSG(LSM, __func__);
    }
    else
    {
        state.ctrl_active.decisions = FC_isFanLevelReduced;
    }
    return ret;
}

inline bool isMode(controller_mode_t mode)
{
    return settings.mode == (uint8_t)mode;
}

#endif // FA_SM_DECISIONS_H
