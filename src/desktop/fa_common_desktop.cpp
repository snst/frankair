#include "fa_common.h"
#include <sys/time.h>
#include <stdio.h>

uint32_t getMillis()
{
  struct timeval tp;
  gettimeofday(&tp, NULL);
  uint32_t ms = tp.tv_sec * 1000U + tp.tv_usec / 1000U;
  return ms;
}

void rebootTarget()
{
}