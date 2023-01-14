// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_FAN_H
#define FA_FAN_H

#include "fa_common.h"

void fanSetup();
void fanSetLevelFresh(uint8_t level);
void fanSetLevelExhaust(uint8_t level);
void fanSetLevelFrost(uint8_t level);
void fanSetLevelFreshAndExhaust(uint8_t level);
void fanSetPWM(uint8_t gpio, uint8_t val);

#endif // FA_FAN_H
