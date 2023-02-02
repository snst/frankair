// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_CALC_H
#define FA_CALC_H

#include <stdint.h>

float convertRelativeToAbsoluteHumidity(float temperature_C, float humidity);
float calcCorrectedValue(float rawV, float rawL, float rawH, float refL, float refH);
void sensorsProcessValues();

#endif // FA_CALC_H
