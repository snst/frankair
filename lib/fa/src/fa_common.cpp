// Copyright (c) 2023 by Stefan Schmidt
#include "fa_common.h"
#include "fa_log.h"
#include "fa_led.h"
#include "fa_sm.h"
#include <cstring>
#include <math.h>

bool g_update_stream = false;

float mapValue(float x, float in_min, float in_max, float out_min, float out_max)
{
  const float run = in_max - in_min;
  if (run == 0)
  {
    return 0;
  }
  const float rise = out_max - out_min;
  const float delta = x - in_min;
  return ((delta * rise)) / run + out_min;
}

uint8_t adjustFanLevelToValidRange(uint8_t level)
{
  level = toRange(level, FAN_LEVEL_OFF, FAN_LEVEL_MAX);
  return level;
}

void addDurationMS(fa_duration_t &duration, uint32_t delta_ms)
{
  duration.ms += delta_ms;
  duration.sec += (duration.ms / 1000U);
  duration.ms = duration.ms % 1000U;
}

bool isInRange(float i, float a, float b)
{
  return ((i >= a && i <= b) || (i >= b && i <= a));
}

void showCmdFeedback()
{
  ledBlink(CMD_FEEDBACK_BLINK_CNT);
}

float round2(float val)
{
  return roundf(val * 100.0f) / 100.0f;
}

float calcAverage(float x1, float x2)
{
    return (x1 + x2) / 2.0f;
}

float calcAverage(float x1, float x2, float x3, float x4)
{
    return (x1 + x2 + x3 + x4) / 4.0f;
}

void filterValue(float &val, float measurement)
{
    float alpha = smInState(fa_sm_gen_StateId::fa_sm_gen_StateId_SNIFF) ? settings.measurement_alpha_sniff : settings.measurement_alpha_on;
    val = (alpha * measurement) + ((1.0f - alpha) * val);
}
