// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_COMMON_H
#define FA_COMMON_H

#include <stdint.h>
#include "fa_defines.h"
#include "fa_log.h"

#define toRange(x, min, max) (x < min ? min : (x > max ? max : x))
extern bool force_update;

uint32_t getMillis();
bool intervalCheckSec(uint32_t &last, uint32_t sec);
bool intervalCheckMS(uint32_t &last, uint32_t ms);
void intervalUpdate();
void intervalReset(uint32_t &val);
uint32_t now();

template <typename T>
bool updateIfChanged(T &dest, T src)
{
  if ((dest != src) || force_update)
  {
    dest = src;
    // IMSG("Change ");
    // IMSG(txt, dest);
    return true;
  }
  return false;
}

float mapValue(float x, float in_min, float in_max, float out_min, float out_max);

#endif // FA_COMMON_H
