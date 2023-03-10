// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_COMMON_H
#define FA_COMMON_H

#include <stdint.h>
#include "fa_defines.h"
#include "fa_log.h"

#define toRange(x, min, max) (x < min ? min : (x > max ? max : x))

extern bool force_update_controller;
extern bool update_stream;

uint32_t getMillis();
uint32_t intervalCheckSec(uint32_t &last, uint32_t sec);
uint32_t intervalCheckMS(uint32_t &last, uint32_t ms);
void intervalNowSet(uint32_t ms);
void intervalNowAdd(uint32_t ms);
void intervalUpdate();
void intervalReset(uint32_t &val);
uint32_t now();
uint8_t adjustFanLevelToValidRange(uint8_t level);
float mapValue(float x, float in_min, float in_max, float out_min, float out_max);
bool isInRange(float i, float a, float b);
void addDurationMS(fa_duration_t &duration, uint32_t delta_ms);
void resetDuration(fa_duration_t &duration);
void rebootTarget();
void cmdFeedback();
float round2(float val);
float calcAverage(float x1, float x2);
float calcAverage(float x1, float x2, float x3, float x4);
void filterValue(float &val, float measurement);

template <typename T>
bool updateIfChanged(T &dest, T src, bool &updated)
{
  if (dest != src)
  {
    dest = src;
    updated = true;
    return true;
  }
  return false;
}

#endif // FA_COMMON_H
