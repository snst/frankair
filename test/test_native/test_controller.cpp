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
  settings.ctrl.fan_level_min = FAN_LEVEL_MIN;
  settings.ctrl.humidity_fan_curve.enabled_points = 0U;
  settings.ctrl.temp_fan_curve.enabled_points = 0U;
  settings.ctrl.humidity_fan_ctrl.enabled = true;
  settings.ctrl.humidity_fan_ctrl.rel_min_off = 50;
  state.humidity.rel_exhaust_in = 50;
  state.submode_auto = (uint8_t)controller_submode_auto_t::kOn;
  fanSetLevelExhaust(settings.ctrl.fan_level_max);
  fanSetLevelFresh(settings.ctrl.fan_level_max);
  fanSetLevelFrost(settings.ctrl.fan_level_max);
}

void test_controller_auto_update(void)
{
  resetData();
  controllerModeAutoUpdate();
  TEST_ASSERT_EQUAL_FLOAT(settings.ctrl.fan_level_max, state.actuator.level_fan_fresh);

  resetData();
  state.humidity.rel_exhaust_in = 49;
  controllerModeAutoUpdate();
  TEST_ASSERT_EQUAL_FLOAT(settings.ctrl.fan_level_min, state.actuator.level_fan_fresh);

  resetData();
  settings.ctrl.humidity_fan_ctrl.abs_min_off = 0.5f;
  state.humidity.abs_delta = 0.5f;
  controllerModeAutoUpdate();
  TEST_ASSERT_EQUAL_FLOAT(settings.ctrl.fan_level_max, state.actuator.level_fan_fresh);
  state.humidity.abs_delta = 0.4f;
  controllerModeAutoUpdate();
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
  controllerModeAutoUpdate();
  TEST_ASSERT_EQUAL_FLOAT(settings.ctrl.fan_level_min, state.actuator.level_fan_fresh);
  state.humidity.abs_delta = 1.0f;
  state.submode_auto = (uint8_t)controller_submode_auto_t::kOn;
  controllerModeAutoUpdate();
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
