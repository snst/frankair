// Copyright (c) 2023 by Stefan Schmidt
#include "fa_log.h"
#include <Arduino.h>
#include "fa_settings.h"

#define CHECK_LOG_MASK(x)          \
  if (!(x & fa_settings.log_mask)) \
    return;

void logging_setup()
{
  Serial.begin(9600);
}

void IMSG(uint8_t mask, const char *txt)
{
  CHECK_LOG_MASK(mask);
  Serial.println(txt);
}

void IMSG(uint8_t mask, float val)
{
  CHECK_LOG_MASK(mask);
  Serial.println(val);
}

void IMSG(uint8_t mask, const char *txt, float val)
{
  CHECK_LOG_MASK(mask);
  Serial.print(txt);
  Serial.print(": ");
  Serial.print(val);
  Serial.println();
}

void IMSGHEX(uint8_t mask, uint8_t val)
{
  CHECK_LOG_MASK(mask);
  Serial.println(val, HEX);
}

void IMSG(uint8_t mask, const char *txt, const char *val)
{
  CHECK_LOG_MASK(mask);
  Serial.print(txt);
  Serial.print(": ");
  Serial.print(val);
  Serial.println();
}
