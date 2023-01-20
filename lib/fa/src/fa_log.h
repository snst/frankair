// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_LOG_H
#define FA_LOG_H

#include <stdint.h>
#include "fa_settings.h"

struct fa_log_mask_t
{
  bool error;
  bool info;
  bool debug;
  bool sensor;
  bool controller;
  bool iot;
  bool actuator;
};

extern fa_log_mask_t log_mask;

#define LERROR 1U
#define LINFO 2U
#define LDEBUG 4U
#define LSENSOR 8U
#define LCONTROLLER 16U
#define LIOT 32U
#define LACTUATOR 64U


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

const char* modeToStr(uint8_t mode);
const char* submodeToStr(uint8_t mode);
const char* modeToStr(controller_mode_t mode);
const char* submodeToStr(controller_submode_auto_t mode);
uint8_t getMask();
void setMask(uint8_t mask);
#endif // FA_LOG_H
