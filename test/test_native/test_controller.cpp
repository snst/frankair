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
  fa_settings.log_mask = 0xFF;
  fa_settings.constraints.fan_level_max = 10;
  fa_settings.constraints.fan_level_min = 1;
  fa_settings.constraints.humidity_fan_on = true;
  fa_settings.constraints.temp_fan_on = true;
  fa_settings.constraints.humidity_on = true;
  fan_set_power_exhaust(fa_settings.constraints.fan_level_max);
  fan_set_power_fresh(fa_settings.constraints.fan_level_max);
  fan_set_power_frost(fa_settings.constraints.fan_level_max);
}

void test_controller_auto_update(void)
{
  resetData();
  fa_settings.constraints.humidity.rel_min_start = 50;
  fa_state.humidity.rel_exaust_in = 50;
  controller_auto_update();
  TEST_ASSERT_EQUAL_FLOAT(fa_settings.constraints.fan_level_max, fa_state.actuator.power_fan_fresh);
  fa_state.humidity.rel_exaust_in = 49;
  controller_auto_update();
  TEST_ASSERT_EQUAL_FLOAT(fa_settings.constraints.fan_level_min, fa_state.actuator.power_fan_fresh);

  resetData();
  fa_settings.constraints.humidity.abs_min_stop = 0.5f;
  fa_state.humidity.abs_delta = 0.5f;
  controller_auto_update();
  TEST_ASSERT_EQUAL_FLOAT(fa_settings.constraints.fan_level_max, fa_state.actuator.power_fan_fresh);
  fa_state.humidity.abs_delta = 0.4f;
  controller_auto_update();
  TEST_ASSERT_EQUAL_FLOAT(fa_settings.constraints.fan_level_min, fa_state.actuator.power_fan_fresh);

  resetData();
  fan_set_power_fresh(fa_settings.constraints.fan_level_min);
  fa_settings.constraints.humidity.abs_min_start = 1.0f;
  fa_state.humidity.abs_delta = 0.9f;
  controller_auto_update();
  TEST_ASSERT_EQUAL_FLOAT(fa_settings.constraints.fan_level_min, fa_state.actuator.power_fan_fresh);
  fa_state.humidity.abs_delta = 1.0f;
  controller_auto_update();
  TEST_ASSERT_EQUAL_FLOAT(fa_settings.constraints.fan_level_max, fa_state.actuator.power_fan_fresh);
}
