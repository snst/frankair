// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_CONTROLLER_H
#define FA_CONTROLLER_H

#include "fa_common.h"
#include "fa_structs.h"

void controllerSetup();
void controllerUpdate();
void controllerModeAutoUpdate();
void controllerStartSniffing();
void controllerStartWait();
bool calcFanOffHumidity(uint8_t &fan_level);
bool calcFanLevelByHumidityCurve(uint8_t &fan_level);
bool calcFanLevelByTempCurve(uint8_t &fan_level);
void controllerModeAutoChangeSubMode(controller_submode_auto_t submode);
uint8_t calcFanFrostLevel();
void controllerStopSniffing();
void controllerModeAutoOn();
void controllerModeAutoWait();
void controllerModeAutoSniff();

#endif // FA_CONTROLLER_H
