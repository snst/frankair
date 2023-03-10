// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_ERROR_H
#define FA_ERROR_H

#include <stdint.h>

#define ERROR_SENSOR_FRESH_IN 1U
#define ERROR_SENSOR_EXHAUST_OUT 2U
#define ERROR_SENSOR_EXHAUST_IN 4U
#define ERROR_SENSOR_FRESH_OUT 8U

void errorUpdate();
void errorClear();
void errorSet(uint8_t error);

#endif // FA_ERROR_H
