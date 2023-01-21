// Copyright (c) 2023 by Stefan Schmidt
#include "fa_log.h"
#include <Arduino.h>

void loggingSetup()
{
  Serial.begin(9600U);
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
  Serial.print(val);
  Serial.println();
}

void IMSG(uint8_t mask, const char *txt1, const char *txt2, const char *txt3, const char *txt4)
{
  CHECK_LOG_MASK(mask);
  Serial.print(txt1);
  Serial.print(txt2);
  Serial.print(txt3);
  Serial.println(txt4);
}
