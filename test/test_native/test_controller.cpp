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
  settings.ctrl.fan_level_max = 10;
  settings.ctrl.fan_level_min = 1;
  settings.ctrl.humidity_fan_enabled = true;
  settings.ctrl.temp_fan_enabled = true;
  settings.ctrl.humidity_enabled = true;
  fanSetLevelExhaust(settings.ctrl.fan_level_max);
  fanSetLevelFresh(settings.ctrl.fan_level_max);
  fanSetLevelFrost(settings.ctrl.fan_level_max);
}

void test_controller_auto_update(void)
{
  resetData();
  settings.ctrl.humidity.rel_min_start = 50;
  state.humidity.rel_exaust_in = 50;
  controllerModeAutoUpdate();
  TEST_ASSERT_EQUAL_FLOAT(settings.ctrl.fan_level_max, state.actuator.level_fan_fresh);
  state.humidity.rel_exaust_in = 49;
  controllerModeAutoUpdate();
  TEST_ASSERT_EQUAL_FLOAT(settings.ctrl.fan_level_min, state.actuator.level_fan_fresh);

  resetData();
  settings.ctrl.humidity.abs_min_stop = 0.5f;
  state.humidity.abs_delta = 0.5f;
  controllerModeAutoUpdate();
  TEST_ASSERT_EQUAL_FLOAT(settings.ctrl.fan_level_max, state.actuator.level_fan_fresh);
  state.humidity.abs_delta = 0.4f;
  controllerModeAutoUpdate();
  TEST_ASSERT_EQUAL_FLOAT(settings.ctrl.fan_level_min, state.actuator.level_fan_fresh);

  resetData();
  fanSetLevelFresh(settings.ctrl.fan_level_min);
  settings.ctrl.humidity.abs_min_start = 1.0f;
  state.humidity.abs_delta = 0.9f;
  controllerModeAutoUpdate();
  TEST_ASSERT_EQUAL_FLOAT(settings.ctrl.fan_level_min, state.actuator.level_fan_fresh);
  state.humidity.abs_delta = 1.0f;
  controllerModeAutoUpdate();
  TEST_ASSERT_EQUAL_FLOAT(settings.ctrl.fan_level_max, state.actuator.level_fan_fresh);
}
