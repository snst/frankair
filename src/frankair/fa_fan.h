// Copyright (c) 2022 by Stefan Schmidt
#ifndef FA_FAN_H
#define FA_FAN_H

void fan_setup();
void fan_set_power_fresh(uint8_t val);
void fan_set_power_exhaust(uint8_t val);
void fan_set_power_frost(uint8_t val);


#endif // FA_FAN_H
