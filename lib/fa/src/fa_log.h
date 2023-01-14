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
#define LM_MODE 32U

#define CHECK_LOG_MASK(x)       \
  if (!(x & settings.log_mask)) \
    return;

void loggingSetup();
void IMSG(uint8_t mask, float val);
void IMSG(uint8_t mask, const char *txt);
void IMSG(uint8_t mask, const char *txt1, const char *txt2, const char *txt3, const char *txt4);
void IMSG(uint8_t mask, const char *txt, float val);
void IMSG(uint8_t mask, const char *txt, const char *val);
void IMSGHEX(uint8_t mask, uint8_t val);

const char* modeToStr(controller_mode_t mode);
const char* submodeToStr(controller_submode_auto_t mode);

#endif // FA_LOG_H
