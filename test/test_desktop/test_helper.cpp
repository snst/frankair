// Copyright (c) 2023 by Stefan Schmidt
#include "test_helper.h"
#include <unity.h>
#include <stdio.h>
#include <iostream>
#include <cstring>
#include "fa_settings.h"

void resetTest()
{
  memset(&calibration_sensor, 0U, sizeof(calibration_sensor));
  memset(&state_raw, 0U, sizeof(state_raw));
  memset(&state, 0U, sizeof(state));
  memset(&settings, 0U, sizeof(settings));

  state.humidity.rel_exhaust_in = settings.ctrl.simple.rel_min_wait + 1.0f;
  state.sm_state = (uint8_t)fa_sm_gen_StateId::fa_sm_gen_StateId_ROOT;
  state.mode = (uint8_t)controller_mode_t::kAuto;
  settingsDefault();

  settings.log_mask = 0x0;

  fanSetLevelExhaust(0);
  fanSetLevelFresh(0);
  fanSetLevelFrost(0);  
}
