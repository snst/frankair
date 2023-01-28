// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_LED_H
#define FA_LED_H

#include <stdint.h>

bool ledEnable(bool enable);
void ledSetup();
void ledUpdate();
void ledBlink(uint8_t count);

#endif // FA_LED_H
