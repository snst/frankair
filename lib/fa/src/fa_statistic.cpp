// Copyright (c) 2023 by Stefan Schmidt
#include "fa_statistic.h"
#include "fa_defines.h"
#include "fa_structs.h"
#include "fa_common.h"
#include <cstring>
#include "fa_calibration.h"
#include "fa_log.h"

extern fa_state_t state;
fa_statistic_t statistic;

void statisticSetup()
{
    statisticReset();
}

void statisticUpdate(uint32_t delta_ms)
{
    float volume = getMainVolume(state.actuator.level_fan_fresh);
    statistic.volume_m3_per_hour = volume;
    statistic.liter_per_hour = state.humidity.abs_delta * volume / 1000.0f;
    statistic.volume_m3_sum += ((volume * delta_ms) / (60U * 60U * 1000U));
    if (statistic.liter_per_hour > 0.0f)
    {
        statistic.liter_sum += (statistic.liter_per_hour * delta_ms / (60U * 60U * 1000U));
    }
    addDurationMS(statistic.running, delta_ms);
    addDurationMS(state.running, delta_ms);
    //IMSG(LM_COMMON, "delta_ms", delta_ms);
    //IMSG(LM_COMMON, "ms", statistic.running.ms);
    //IMSG(LM_COMMON, "sec", statistic.running.sec);
}

void statisticReset()
{
    memset(&statistic, 0U, sizeof(statistic));
}