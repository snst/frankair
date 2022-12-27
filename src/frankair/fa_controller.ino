// Copyright (c) 2022 by Stefan Schmidt
#include "fa_controller.h"
#include "fa_common.h"

static uint32_t controller_now = 0;
fa_controller_t fa_controller;

void controller_setup()
{
}

void controller_update()
{
  if (interval(controller_now, fa_settings.controller_interval_sec))
  {
    IMSG("control!\n");
  }
}
