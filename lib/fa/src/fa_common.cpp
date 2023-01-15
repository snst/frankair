// Copyright (c) 2023 by Stefan Schmidt
#include "fa_common.h"
#include "fa_log.h"

bool force_update = true;
static uint32_t now_ms = 0U;

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

bool intervalCheckSec(uint32_t &last, uint32_t sec)
{
  return intervalCheckMS(last, (1000U * sec));
}

bool intervalCheckMS(uint32_t &last, uint32_t ms)
{
  if (now() - last >= ms)
  {
    last = now();
    return true;
  }
  return false;
}

float mapValue(float x, float in_min, float in_max, float out_min, float out_max) {
    const long run = in_max - in_min;
    if(run == 0){
        return 0;
    }
    const float rise = out_max - out_min;
    const float delta = x - in_min;
    return (delta * rise) / run + out_min;
}