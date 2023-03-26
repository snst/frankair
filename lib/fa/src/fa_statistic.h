// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_STATISTIC_H
#define FA_STATISTIC_H

#include <stdint.h>
#include "fa_structs.h"

extern fa_statistic_t statistic;

void statisticSetup();
void statisticUpdate();
void statisticReset();

#endif // FA_STATISTIC_H
