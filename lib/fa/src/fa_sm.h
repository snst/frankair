// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_SM_H
#define FA_SM_H

#include "fa_structs.h"
#include "fa_defines.h"
#include "fa_sm_gen.h"

void smInit();
void smUpdate();
void smSendEvent(enum fa_sm_gen_EventId event_id, uint32_t ms = 0U);
fa_sm_gen_StateId smGetStateId();
bool smInState(enum fa_sm_gen_StateId state);
const char *smGetStateStr();

#endif // FA_SM_H
