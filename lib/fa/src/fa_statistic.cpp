// Copyright (c) 2023 by Stefan Schmidt
#include "fa_statistic.h"
#include "fa_defines.h"
#include "fa_structs.h"
#include "fa_common.h"
#include <cstring>
#include "fa_calibration.h"
#include "fa_log.h"
#include "fa_timer.h"

fa_statistic_t statistic;

void statisticSetup()
{
    statisticReset();
}

void statisticCalculateEfficiency()
{
    float efficiency = 100.0f;
    float delta_temp = state.temp.exhaust_in - state.temp.fresh_in;
    float inc_temp = state.temp.fresh_out - state.temp.fresh_in;
    if (delta_temp > 0.0f)
    {
        efficiency = 100.0f * inc_temp / delta_temp;
    }
    state.efficiency = toRange(efficiency, 0.0f, 100.0f);
}

void statisticUpdate()
{
    static uint32_t lastMs = now();
    uint32_t delta_ms = now() - lastMs;
    lastMs = now();
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
    statisticCalculateEfficiency();
}

void statisticReset()
{
    memset(&statistic, 0U, sizeof(statistic));
}
