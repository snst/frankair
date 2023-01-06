// Copyright (c) 2022 by Stefan Schmidt
#include "fa_log.h"

void logging_setup()
{
  Serial.begin(9600);
}

void IMSG(const char *txt)
{
  Serial.print(txt);
}

void IMSG(float val)
{
  Serial.print(val);
}

void IMSG(const char *txt, float val)
{
  Serial.print(txt);
  Serial.print(": ");
  Serial.print(val);
  Serial.println();
}

void IMSGHEX(uint8_t val)
{
  Serial.print(val, HEX);
}

void IMSG(const char *txt, const char *val)
{
  Serial.print(txt);
  Serial.print(": ");
  Serial.print(val);
  Serial.println();
}
