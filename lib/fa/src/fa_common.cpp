#include "fa_common.h"
#include "fa_log.h"

bool g_force_update = true;
static uint32_t s_now = 0;

void interval_update()
{
  s_now = getMillis();
}

uint32_t now()
{
  return s_now;
}

bool interval(uint32_t &last, uint32_t delay)
{
  if (s_now - last >= delay)
  {
    last = s_now;
    return true;
  }
  return false;
}
