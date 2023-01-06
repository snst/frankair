// Copyright (c) 2022 by Stefan Schmidt
#ifndef FA_COMMON_H
#define FA_COMMON_H

#include <Arduino.h>
#include <stdint.h>
#include "fa_defines.h"
#include "fa_log.h"

#define toRange(x, min, max) (x < min ? min : (x > max ? max : x))
extern bool g_force_update;

bool interval(uint32_t &last, uint32_t delay);
void interval_update();
uint32_t now();

template <typename T>
bool update_if_changed(T &dest, T src, const char *txt)
{
  if ((dest != src) || g_force_update)
  {
    dest = src;
    // IMSG("Change ");
    // IMSG(txt, dest);
    return true;
  }
  return false;
}

#endif // FA_COMMON_H
