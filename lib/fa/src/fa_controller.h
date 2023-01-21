// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_CONTROLLER_H
#define FA_CONTROLLER_H

#include "fa_common.h"
#include "fa_structs.h"

extern fa_state_t state;
extern fa_state_raw_t state_raw;

void controllerSetup();
void controllerUpdate();
void controllerModeAutoUpdate();
void controllerStartSniff();
void controllerStartWait();
bool calcFanOffHumidity(uint8_t &fan_level);
bool calcFanLevelByHumidityCurve(uint8_t &fan_level);
bool calcFanLevelByTempCurve(uint8_t &fan_level);
void controllerModeAutoChangeSubMode(controller_submode_auto_t submode);
uint8_t calcFanFrostLevel();

#endif // FA_CONTROLLER_H
