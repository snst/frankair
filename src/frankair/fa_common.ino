#include "fa_common.h"

static uint32_t s_now = 0;

void interval_update()
{
  s_now = millis();
}

uint32_t now()
{
  return s_now;
}

bool interval(uint32_t& last, uint32_t delay)
{
  if (s_now - last >= delay)
  {
    last = s_now;
    return true;
  }
  return false;
}