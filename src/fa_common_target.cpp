#include "fa_common.h"
#include <Arduino.h>

uint32_t getMillis()
{
  return millis();
}

void rebootTarget()
{
  ESP.restart();
}
