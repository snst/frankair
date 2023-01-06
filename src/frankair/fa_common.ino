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

bool interval(uint32_t &last, uint32_t delay)
{
  if (s_now - last >= delay)
  {
    last = s_now;
    return true;
  }
  return false;
}

template <typename T>
bool update_if_changed(T &dest, T src, const char *txt)
{
  if ((dest != src) || fa_settings.changed)
  {
    dest = src;
    IMSG("Change ");
    IMSG(txt, dest);
    return true;
  }
  return false;
}