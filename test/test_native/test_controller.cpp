#include <unity.h>
#include <stdio.h>
#include <iostream>
#include <cstring>

#include "fa_controller.h"
#include "fa_settings.h"
#include "fa_fan.h"

void cleanData();

void resetData()
{
  cleanData();
  settings.log_mask = 0xFF;
  settings.ctrl.fan_level_max = FAN_LEVEL_MAX;
  settings.ctrl.fan_level_min = FAN_LEVEL_OFF;
  settings.ctrl.humidity_fan_curve.enabled_points = 0U;
  settings.ctrl.temp_fan_curve.enabled_points = 0U;
  settings.ctrl.humidity_fan_ctrl.enabled = true;
  settings.ctrl.humidity_fan_ctrl.rel_min_off = 50.0f;
  settings.controller_interval_sec = 1U;
  state.humidity.rel_exhaust_in = settings.ctrl.humidity_fan_ctrl.rel_min_off + 1.0f;
  state.submode_auto = (uint8_t)controller_submode_auto_t::kOn;
  settings.mode = (uint8_t)controller_mode_t::kAuto;
  state.mode = (uint8_t)controller_mode_t::kAuto;
  fanSetLevelExhaust(settings.ctrl.fan_level_max);
  fanSetLevelFresh(settings.ctrl.fan_level_max);
  fanSetLevelFrost(settings.ctrl.fan_level_max);
}

void loopController()
{
  controller_now_reset_for_test();
  intervalNowAdd(settings.controller_interval_sec * 1000U);
  controllerUpdate();
  if (force_update_controller)
    controllerUpdate();
}

void test_controller_auto_update(void)
{
  resetData();
  state.humidity.abs_delta = settings.ctrl.humidity_fan_ctrl.abs_min_off + 0.1f;
  loopController();
  TEST_ASSERT_EQUAL_FLOAT(settings.ctrl.fan_level_max, state.actuator.level_fan_fresh);

  resetData();
  state.humidity.rel_exhaust_in = 49;
  loopController();
  TEST_ASSERT_EQUAL_FLOAT(settings.ctrl.fan_level_min, state.actuator.level_fan_fresh);

  resetData();
  settings.ctrl.humidity_fan_ctrl.abs_min_off = 0.5f;
  state.humidity.abs_delta = settings.ctrl.humidity_fan_ctrl.abs_min_off + 0.1f;
  loopController();
  TEST_ASSERT_EQUAL_FLOAT(settings.ctrl.fan_level_max, state.actuator.level_fan_fresh);
  state.humidity.abs_delta = 0.4f;
  loopController();
  TEST_ASSERT_EQUAL_FLOAT(settings.ctrl.fan_level_min, state.actuator.level_fan_fresh);
}

void test_controller_start_from_sniffing(void)
{
  resetData();
  state.submode_auto = (uint8_t)controller_submode_auto_t::kOn;
  fanSetLevelFresh(settings.ctrl.fan_level_min);
  settings.ctrl.humidity_fan_ctrl.abs_min_on = 1.0f;
  settings.ctrl.humidity_fan_ctrl.abs_min_off = 0.5f;
  state.humidity.abs_delta = 0.4f;
  loopController();
  TEST_ASSERT_EQUAL_FLOAT(settings.ctrl.fan_level_min, state.actuator.level_fan_fresh);
  state.humidity.abs_delta = 1.0f;
  state.submode_auto = (uint8_t)controller_submode_auto_t::kOn;
  loopController();
  TEST_ASSERT_EQUAL_FLOAT(settings.ctrl.fan_level_max, state.actuator.level_fan_fresh);
}

void test_controller_humidity_curve(void)
{
  resetData();
  const uint8_t start_level = 10U;
  uint8_t level = start_level;
  settings.ctrl.humidity_fan_curve.enabled = true;
  settings.ctrl.humidity_fan_curve.enabled_points = 0U;
  settings.ctrl.humidity_fan_curve.item[0].val = 2.0f;
  settings.ctrl.humidity_fan_curve.item[0].level = 1U;
  settings.ctrl.humidity_fan_curve.item[1].val = 4.0f;
  settings.ctrl.humidity_fan_curve.item[1].level = 3U;
  settings.ctrl.humidity_fan_curve.item[2].val = 9.0f;
  settings.ctrl.humidity_fan_curve.item[2].level = 5U;

  settings.ctrl.humidity_fan_curve.enabled_points = 2U;

  level = start_level;
  state.humidity.abs_delta = 1.0f;
  TEST_ASSERT_TRUE(calcFanLevelByHumidityCurve(level));
  TEST_ASSERT_EQUAL(1U, level);

  level = start_level;
  state.humidity.abs_delta = 2.0f;
  TEST_ASSERT_TRUE(calcFanLevelByHumidityCurve(level));
  TEST_ASSERT_EQUAL(1U, level);

  level = start_level;
  state.humidity.abs_delta = 3.0f;
  TEST_ASSERT_TRUE(calcFanLevelByHumidityCurve(level));
  TEST_ASSERT_EQUAL(2U, level);

  level = start_level;
  state.humidity.abs_delta = 4.0f;
  TEST_ASSERT_TRUE(calcFanLevelByHumidityCurve(level));
  TEST_ASSERT_EQUAL(3U, level);

  level = start_level;
  state.humidity.abs_delta = 4.1f;
  TEST_ASSERT_TRUE(calcFanLevelByHumidityCurve(level));
  TEST_ASSERT_EQUAL(3U, level);

  settings.ctrl.humidity_fan_curve.enabled_points = 3U;
  level = start_level;
  state.humidity.abs_delta = 3.0f;
  TEST_ASSERT_TRUE(calcFanLevelByHumidityCurve(level));
  TEST_ASSERT_EQUAL(2U, level);

  level = start_level;
  state.humidity.abs_delta = 0.0f;
  TEST_ASSERT_TRUE(calcFanLevelByHumidityCurve(level));
  TEST_ASSERT_EQUAL(1U, level);

  level = start_level;
  state.humidity.abs_delta = -3.0f;
  TEST_ASSERT_TRUE(calcFanLevelByHumidityCurve(level));
  TEST_ASSERT_EQUAL(1U, level);

  level = start_level;
  state.humidity.abs_delta = 6.5f;
  TEST_ASSERT_TRUE(calcFanLevelByHumidityCurve(level));
  TEST_ASSERT_EQUAL(4U, level);

  level = start_level;
  state.humidity.abs_delta = 9.0f;
  TEST_ASSERT_TRUE(calcFanLevelByHumidityCurve(level));
  TEST_ASSERT_EQUAL(5U, level);

  level = start_level;
  state.humidity.abs_delta = 9.1f;
  TEST_ASSERT_TRUE(calcFanLevelByHumidityCurve(level));
  TEST_ASSERT_EQUAL(5U, level);

  settings.ctrl.humidity_fan_curve.enabled_points = 4U;
  settings.ctrl.humidity_fan_curve.item[0].val = 4.0f;
  settings.ctrl.humidity_fan_curve.item[0].level = 10U;
  settings.ctrl.humidity_fan_curve.item[1].val = 3.0f;
  settings.ctrl.humidity_fan_curve.item[1].level = 8U;
  settings.ctrl.humidity_fan_curve.item[2].val = 2.0f;
  settings.ctrl.humidity_fan_curve.item[2].level = 5U;
  settings.ctrl.humidity_fan_curve.item[3].val = 0.5f;
  settings.ctrl.humidity_fan_curve.item[3].level = 0U;
  level = start_level;
  state.humidity.abs_delta = 1.06f;
  TEST_ASSERT_TRUE(calcFanLevelByHumidityCurve(level));
  TEST_ASSERT_EQUAL(2U, level);
}

void test_controller_frost_curve(void)
{
  resetData();
  state.actuator.open_flap_frost = 1U;
  settings.ctrl.frost_flap_ctrl.enabled = true;

  settings.ctrl.frost_fan_curve.enabled = true;
  settings.ctrl.frost_fan_curve.enabled_points = 2U;
  settings.ctrl.frost_fan_curve.item[0].val = 0.0f;
  settings.ctrl.frost_fan_curve.item[0].level = 0U;
  settings.ctrl.frost_fan_curve.item[1].val = -4.0f;
  settings.ctrl.frost_fan_curve.item[1].level = 2U;
  settings.ctrl.frost_fan_curve.item[2].val = -6.0f;
  settings.ctrl.frost_fan_curve.item[2].level = 4U;

  state.temp.fresh_in = 10.0f;
  TEST_ASSERT_EQUAL(0U, calcFanFrostLevel());
  state.temp.fresh_in = 0.0f;
  TEST_ASSERT_EQUAL(0U, calcFanFrostLevel());
  state.temp.fresh_in = -2.0f;
  TEST_ASSERT_EQUAL(1U, calcFanFrostLevel());
  state.temp.fresh_in = -4.0f;
  TEST_ASSERT_EQUAL(2U, calcFanFrostLevel());
  state.temp.fresh_in = -5.0f;
  TEST_ASSERT_EQUAL(2U, calcFanFrostLevel());
  state.temp.fresh_in = -50.0f;
  TEST_ASSERT_EQUAL(2U, calcFanFrostLevel());

  settings.ctrl.frost_fan_curve.enabled_points = 3U;
  state.temp.fresh_in = -4.0f;
  TEST_ASSERT_EQUAL(2U, calcFanFrostLevel());
  state.temp.fresh_in = -5.0f;
  TEST_ASSERT_EQUAL(3U, calcFanFrostLevel());
  state.temp.fresh_in = -50.0f;
  TEST_ASSERT_EQUAL(4U, calcFanFrostLevel());
}

fa_state_t expect_state;

bool check_auto_step()
{
  loopController();
  bool ret = true;
  ret &= (expect_state.actuator.level_fan_exhaust == state.actuator.level_fan_exhaust);
  // TEST_ASSERT_TRUE(ret);
  // ret &= (expect_state.actuator.level_fan_fresh == state.actuator.level_fan_fresh);
  // TEST_ASSERT_TRUE(ret);
  ret &= (expect_state.actuator.level_fan_frost == state.actuator.level_fan_frost);
  // TEST_ASSERT_TRUE(ret);
  ret &= (expect_state.actuator.open_flap_frost == state.actuator.open_flap_frost);
  // TEST_ASSERT_TRUE(ret);
  ret &= (expect_state.submode_auto == state.submode_auto);

  return ret;
}

void test_controller_sequence(void)
{
  memset(&state, 0U, sizeof(state));
  memset(&expect_state, 0U, sizeof(expect_state));
  settingsDefault();
  state.mode = (uint8_t)controller_mode_t::kAuto;
  state.submode_auto = (uint8_t)controller_submode_auto_t::kOn;
  state.humidity.rel_exhaust_in = settings.ctrl.humidity_fan_ctrl.rel_min_off + 1.0f;
  state.humidity.abs_delta = settings.ctrl.humidity_fan_ctrl.abs_min_off + 1;
  state.temp.exhaust_in = 20.0f;

  // flap off
  settings.ctrl.frost_flap_ctrl.enabled = false;
  settings.manual.open_flap_frost = FLAP_LEVEL_CLOSE;
  expect_state.actuator.open_flap_frost = settings.manual.open_flap_frost;
  expect_state.actuator.level_fan_exhaust = 3U;
  expect_state.submode_auto = (uint8_t)controller_submode_auto_t::kOn;
  ;
  TEST_ASSERT_TRUE(check_auto_step());

  // auto flap
  settings.ctrl.frost_flap_ctrl.enabled = true;
  settings.ctrl.frost_flap_ctrl.temp_min_open = -2.0f;
  settings.ctrl.frost_flap_ctrl.temp_min_close = 4.0f;

  // between min open and close => do nothing
  state.actuator.open_flap_frost = 3U;
  state.temp.fresh_in = (settings.ctrl.frost_flap_ctrl.temp_min_open + settings.ctrl.frost_flap_ctrl.temp_min_close) / 2U;
  expect_state.actuator.open_flap_frost = 3U;
  state.submode_auto = (uint8_t)controller_submode_auto_t::kOn;
  TEST_ASSERT_TRUE(check_auto_step());

  // set to settings.ctrl.frost_flap_ctrl.temp_min_open => open flap
  state.temp.fresh_in = settings.ctrl.frost_flap_ctrl.temp_min_open;
  expect_state.actuator.open_flap_frost = settings.ctrl.frost_flap_ctrl.level_open;
  expect_state.actuator.level_fan_frost = 4U;
  state.submode_auto = (uint8_t)controller_submode_auto_t::kOn;
  TEST_ASSERT_TRUE(check_auto_step());

  // set between settings.ctrl.frost_flap_ctrl.temp_min_open/temp_min_close => still open flap
  state.temp.fresh_in = (settings.ctrl.frost_flap_ctrl.temp_min_open + settings.ctrl.frost_flap_ctrl.temp_min_close) / 2U;
  expect_state.actuator.open_flap_frost = settings.ctrl.frost_flap_ctrl.level_open;
  expect_state.actuator.level_fan_frost = 0U;
  state.submode_auto = (uint8_t)controller_submode_auto_t::kOn;
  TEST_ASSERT_TRUE(check_auto_step());

  // set to settings.ctrl.frost_flap_ctrl.temp_min_close => close flap
  state.temp.fresh_in = settings.ctrl.frost_flap_ctrl.temp_min_close;
  expect_state.actuator.open_flap_frost = FLAP_LEVEL_CLOSE;
  state.submode_auto = (uint8_t)controller_submode_auto_t::kOn;
  TEST_ASSERT_TRUE(check_auto_step());

  state.submode_auto = (uint8_t)controller_submode_auto_t::kOn;
  settings.ctrl.humidity_fan_ctrl.enabled = false;
  state.temp.exhaust_in = 100.0f;
  state.humidity.abs_delta = 100.0f;
  expect_state.submode_auto = (uint8_t)controller_submode_auto_t::kOn;
  expect_state.actuator.level_fan_exhaust = settings.ctrl.temp_fan_curve.item[0].level;
  TEST_ASSERT_TRUE(check_auto_step());

  state.temp.exhaust_in = settings.ctrl.temp_fan_curve.item[1].val;
  expect_state.actuator.level_fan_exhaust = settings.ctrl.temp_fan_curve.item[1].level;
  TEST_ASSERT_TRUE(check_auto_step());

  // temp between two points
  state.temp.exhaust_in = (settings.ctrl.temp_fan_curve.item[0].val + settings.ctrl.temp_fan_curve.item[1].val) / 2.0f;
  expect_state.actuator.level_fan_exhaust = (settings.ctrl.temp_fan_curve.item[0].level + settings.ctrl.temp_fan_curve.item[1].level) / 2.0f;
  TEST_ASSERT_TRUE(check_auto_step());

  // temp very low, enabled_points-1 not to completely turn off fans
  state.temp.exhaust_in = -100.0f;
  settings.ctrl.temp_fan_curve.enabled_points--;
  expect_state.actuator.level_fan_exhaust = settings.ctrl.temp_fan_curve.item[settings.ctrl.temp_fan_curve.enabled_points - 1U].level;
  TEST_ASSERT_TRUE(check_auto_step());

  // temp very low, last temp point turns off fan => kWait
  settings.ctrl.temp_fan_curve.enabled_points++;
  expect_state.actuator.level_fan_exhaust = settings.ctrl.temp_fan_curve.item[settings.ctrl.temp_fan_curve.enabled_points - 1U].level;
  expect_state.submode_auto = (uint8_t)controller_submode_auto_t::kWait;
  TEST_ASSERT_TRUE(check_auto_step());

  state.temp.exhaust_in = settings.ctrl.temp_fan_curve.item[1].val;
  expect_state.actuator.level_fan_exhaust = settings.ctrl.temp_fan_curve.item[1].level;
  expect_state.submode_auto = (uint8_t)controller_submode_auto_t::kOn;
  controllerStartSniffing();
  controllerModeAutoOn();
  TEST_ASSERT_TRUE(check_auto_step());

  // humidity further reduces fan level
  state.humidity.abs_delta = settings.ctrl.humidity_fan_curve.item[2].val;
  expect_state.actuator.level_fan_exhaust = settings.ctrl.humidity_fan_curve.item[2].level;
  TEST_ASSERT_TRUE(check_auto_step());

  // stop fan because rel humidity too low
  settings.ctrl.humidity_fan_ctrl.enabled = true;
  state.humidity.rel_exhaust_in = settings.ctrl.humidity_fan_ctrl.rel_min_off;
  expect_state.actuator.level_fan_exhaust = settings.ctrl.fan_level_min;
  expect_state.submode_auto = (uint8_t)controller_submode_auto_t::kWait;
  TEST_ASSERT_TRUE(check_auto_step());

  state.humidity.rel_exhaust_in = settings.ctrl.humidity_fan_ctrl.rel_min_off + 0.1f;
  state.humidity.abs_delta = settings.ctrl.humidity_fan_ctrl.abs_min_on - 0.1f;
  controllerStartSniffing();
  controllerModeAutoOn();
  TEST_ASSERT_TRUE(check_auto_step());

  state.temp.exhaust_in = 100.0f;
  state.humidity.abs_delta = settings.ctrl.humidity_fan_curve.item[2].val;
  TEST_ASSERT_TRUE(settings.ctrl.humidity_fan_curve.item[2].val >= settings.ctrl.humidity_fan_ctrl.abs_min_on);
  expect_state.actuator.level_fan_exhaust = settings.ctrl.humidity_fan_curve.item[2].level;
  expect_state.submode_auto = (uint8_t)controller_submode_auto_t::kOn;
  controllerStartSniffing();
  controllerModeAutoOn();
  TEST_ASSERT_TRUE(check_auto_step());
}