// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_FAN_H
#define FA_FAN_H

#include "fa_common.h"

void fan_setup();
void fan_set_power_fresh(uint8_t val);
void fan_set_power_exhaust(uint8_t val);
void fan_set_power_frost(uint8_t val);
void fan_set(uint8_t gpio, uint8_t val);

#endif // FA_FAN_H
