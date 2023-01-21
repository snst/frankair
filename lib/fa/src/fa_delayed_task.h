// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_DELAYED_TASK_H
#define FA_DELAYED_TASK_H

#include "fa_common.h"

typedef void (*delayedTaskFunc)(void);

void delayedTaskSet(delayedTaskFunc func, uint32_t ms);
void delayedTaskSetup();
void delayedTaskUpdate();
void delayedTaskReboot();

#endif // FA_DELAYED_TASK_H
