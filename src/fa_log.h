// Copyright (c) 2022 by Stefan Schmidt
#ifndef FA_LOG_H
#define FA_LOG_H

#include "fa_common.h"

void logging_setup();
void IMSG(float val);
void IMSG(const char *txt);
void IMSG(const char *txt, float val);
void IMSG(const char *txt, const char *val);
void IMSGHEX(uint8_t val);

#endif // FA_LOG_H
