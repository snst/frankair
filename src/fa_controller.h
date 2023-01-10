// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_CONTROLLER_H
#define FA_CONTROLLER_H

#include "fa_common.h"
#include "fa_structs.h"

extern fa_state_t fa_state;
extern fa_state_raw_t fa_state_raw;

void controller_setup();
void controller_update();

#endif // FA_CONTROLLER_H
