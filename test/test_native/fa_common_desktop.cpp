#include "fa_common.h"
#include <sys/time.h>
#include <stdio.h>

uint32_t desktop_ms = 0U;

uint32_t getMillis()
{
  return desktop_ms;
  /*
  struct timeval tp;
  gettimeofday(&tp, NULL);
  uint32_t ms = tp.tv_sec * 1000U + tp.tv_usec / 1000U;
  return ms;
  */
}
