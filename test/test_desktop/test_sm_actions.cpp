// Copyright (c) 2023 by Stefan Schmidt
#include "test_helper.h"
#include "fa_sm_actions.h"

#define FLAP_INITIAL 0U
#define FROST_INITIAL 0U

void test_smActionModeOnAdjustFrost_off()
{
  // test that manual frost settings used when auto frost flap disabled
  resetTest();
  settings.manual.open_flap_frost = 3U;
  settings.manual.level_fan_frost = 4U;
  state.actuator.open_flap_frost = FLAP_INITIAL;
  state.actuator.level_fan_frost = FROST_INITIAL;
  settings.ctrl.frost_flap_ctrl.enabled = false;
  smActionModeOnAdjustFrost();
  TEST_ASSERT_EQUAL(3U, state.actuator.open_flap_frost);
  TEST_ASSERT_EQUAL(4U, state.actuator.level_fan_frost);
}

void test_smActionModeOnAdjustFrost_on()
{
  // don't open if not cold enough
  resetTest();
  state.actuator.open_flap_frost = FLAP_INITIAL;
  state.actuator.level_fan_frost = FROST_INITIAL;
  settings.ctrl.frost_flap_ctrl.enabled = true;
  settings.ctrl.frost_flap_ctrl.temp_min_open = -0.7;
  settings.ctrl.frost_flap_ctrl.temp_min_close = 1.5;

  state.temp.fresh_in = -0.6;
  smActionModeOnAdjustFrost();
  TEST_ASSERT_EQUAL(FLAP_INITIAL, state.actuator.open_flap_frost);
  TEST_ASSERT_EQUAL(FROST_INITIAL, state.actuator.level_fan_frost);

  // open flap
  state.temp.fresh_in = -0.7;
  smActionModeOnAdjustFrost();
  TEST_ASSERT_EQUAL(settings.ctrl.frost_flap_ctrl.level_open, state.actuator.open_flap_frost);
  TEST_ASSERT_EQUAL(calcFanFrostLevel(), state.actuator.level_fan_frost);

  // dont'change
  state.temp.fresh_in = 1.0;
  smActionModeOnAdjustFrost();
  TEST_ASSERT_EQUAL(settings.ctrl.frost_flap_ctrl.level_open, state.actuator.open_flap_frost);
  TEST_ASSERT_EQUAL(calcFanFrostLevel(), state.actuator.level_fan_frost);

  // close flap
  state.temp.fresh_in = 1.5;
  smActionModeOnAdjustFrost();
  TEST_ASSERT_EQUAL(FLAP_LEVEL_CLOSE, state.actuator.open_flap_frost);
  TEST_ASSERT_EQUAL(FAN_LEVEL_OFF, state.actuator.level_fan_frost);
}
