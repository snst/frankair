// Copyright (c) 2023 by Stefan Schmidt
#include "test_helper.h"

void test_smEnterInitialState()
{
  resetTest();

  settings.mode = (uint8_t)controller_mode_t::kAuto;
  smInit();
  TEST_ASSERT_TRUE(smInState(fa_sm_gen_StateId::fa_sm_gen_StateId_SNIFF));

  settings.mode = (uint8_t)controller_mode_t::kOff;
  smInit();
  TEST_ASSERT_TRUE(smInState(fa_sm_gen_StateId::fa_sm_gen_StateId_OFF));

  settings.mode = (uint8_t)controller_mode_t::kManual;
  smInit();
  TEST_ASSERT_TRUE(smInState(fa_sm_gen_StateId::fa_sm_gen_StateId_MANUAL));  
}
