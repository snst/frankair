// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_CONTROLLER_H
#define FA_CONTROLLER_H

#include "fa_common.h"
#include "fa_structs.h"

bool calcFanLevelByHumidityCurve(uint8_t &fan_level);
bool calcFanLevelByTempCurve(uint8_t &fan_level);
uint8_t calcFanFrostLevel();
bool reduceFanLevel(uint8_t &dest, uint8_t val);

#endif // FA_CONTROLLER_H
