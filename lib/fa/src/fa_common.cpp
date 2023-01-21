// Copyright (c) 2023 by Stefan Schmidt
#include "fa_common.h"
#include "fa_log.h"
#include <cstring>

bool force_update = true;
static uint32_t now_ms = 0U;

void intervalNowSet(uint32_t ms)
{
  now_ms = ms;
}

void intervalNowAdd(uint32_t ms)
{
  now_ms += ms;
}

void intervalUpdate()
{
  now_ms = getMillis();
}

uint32_t now()
{
  return now_ms;
}

void intervalReset(uint32_t &val)
{
  val = now();
}

uint32_t intervalCheckSec(uint32_t &last, uint32_t sec)
{
  return intervalCheckMS(last, (1000U * sec));
}

uint32_t intervalCheckMS(uint32_t &last, uint32_t ms)
{
  uint32_t diff = now() - last;
  if (diff >= ms)
  {
    last = now();
    return diff;
  }
  return 0U;
}

float mapValue(float x, float in_min, float in_max, float out_min, float out_max)
{
  const long run = in_max - in_min;
  if (run == 0)
  {
    return 0;
  }
  const float rise = out_max - out_min;
  const float delta = x - in_min;
  return (delta * rise) / run + out_min;
}

uint8_t adjustFanLevelToValidRange(uint8_t level)
{
  level = toRange(level, FAN_LEVEL_MIN, FAN_LEVEL_MAX);
  return level;
}

uint32_t convertMS(uint32_t &ms, uint32_t mod)
{
  uint32_t ret = ms / mod;
  ms = ms % mod;
  return ret;
}

void addDurationMS(fa_duration_t &duration, uint32_t delta_ms)
{
  duration.ms += delta_ms;
  duration.sec += (duration.ms / 1000U);
  duration.ms = duration.ms % 1000U;
}

void resetDuration(fa_duration_t &duration)
{
  memset(&duration, 0U, sizeof(duration));
}

bool isInRange(float i, float a, float b)
{
  return ((i >= a && i <= b) || (i >= b && i <= a));
}
