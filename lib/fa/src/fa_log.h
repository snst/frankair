// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_LOG_H
#define FA_LOG_H

#include <stdint.h>
#include "fa_settings.h"

#define LM_COMMON 1U
#define LM_SENSOR 2U
#define LM_THING 4U
#define LM_SETTING 8U
#define LM_FAN 16U


#define CHECK_LOG_MASK(x)          \
  if (!(x & fa_settings.log_mask)) \
    return;

void logging_setup();
void IMSG(uint8_t mask, float val);
void IMSG(uint8_t mask, const char *txt);
void IMSG(uint8_t mask, const char *txt, float val);
void IMSG(uint8_t mask, const char *txt, const char *val);
void IMSGHEX(uint8_t mask, uint8_t val);

#endif // FA_LOG_H
