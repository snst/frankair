// Copyright (c) 2023 by Stefan Schmidt
#include "test_helper.h"

void test_convertRelativeToAbsoluteHumidity()
{
  TEST_ASSERT_EQUAL_FLOAT(9.207461, convertRelativeToAbsoluteHumidity(25, 40));
  TEST_ASSERT_EQUAL_FLOAT(13.81119, convertRelativeToAbsoluteHumidity(25, 60));
  TEST_ASSERT_EQUAL_FLOAT(18.41492, convertRelativeToAbsoluteHumidity(25, 80));
  TEST_ASSERT_EQUAL_FLOAT(2.717554, convertRelativeToAbsoluteHumidity(5, 40));
  TEST_ASSERT_EQUAL_FLOAT(4.076331, convertRelativeToAbsoluteHumidity(5, 60));
  TEST_ASSERT_EQUAL_FLOAT(5.435108, convertRelativeToAbsoluteHumidity(5, 80));
  TEST_ASSERT_EQUAL_FLOAT(3.878644, convertRelativeToAbsoluteHumidity(0, 80));
  TEST_ASSERT_EQUAL_FLOAT(0.0001727296, convertRelativeToAbsoluteHumidity(20, 0.001));
  TEST_ASSERT_EQUAL_FLOAT(0, convertRelativeToAbsoluteHumidity(20, 0));
}

void test_calcAverage()
{
  TEST_ASSERT_EQUAL_FLOAT(5.5f, calcAverage(3.3f, 7.7f));
  TEST_ASSERT_EQUAL_FLOAT(5.1f, calcAverage(3.3f, 7.7f, 1.5, 7.9));
}

void test_calcCorrectedValue()
{
  TEST_ASSERT_EQUAL_FLOAT(4.0f, calcCorrectedValue(1.75, 0.5, 2.5, 1.5, 5.5));
  TEST_ASSERT_EQUAL_FLOAT(2.5f, calcCorrectedValue(1, 0.5, 2.5, 1.5, 5.5));
}

void test_filterValue()
{
  resetTest();
  g_state_machine.state_id = fa_sm_gen_StateId::fa_sm_gen_StateId_SNIFF;
  settings.measurement_alpha_sniff = 1.0f;
  float val = 10.0f;
  filterValue(val, 5.0f);
  TEST_ASSERT_EQUAL_FLOAT(5.0f, val);

  g_state_machine.state_id = fa_sm_gen_StateId::fa_sm_gen_StateId_ON;
  settings.measurement_alpha_on = 0.5f;
  val = 10.0f;
  filterValue(val, 5.0f);
  TEST_ASSERT_EQUAL_FLOAT(7.5f, val);
}

void test_correctTempWithCalibrationData()
{
  resetTest();
  calibration_sensor.exhaust_in_temp.min = 10.0f;
  calibration_sensor.exhaust_in_temp.max = 30.0f;
  calibration_sensor.exhaust_out_temp.min = 20.0f;
  calibration_sensor.exhaust_out_temp.max = 40.0f;
  calibration_sensor.fresh_in_temp.min = -10.0f;
  calibration_sensor.fresh_in_temp.max = 30.0f;
  calibration_sensor.fresh_out_temp.min = 10.0f;
  calibration_sensor.fresh_out_temp.max = 30.0f;
  calibration_sensor.ref_temp.min = 40.0f;
  calibration_sensor.ref_temp.max = 50.0f;
  fa_temp_t dest, in;
  in.exhaust_in = 20.0f;
  in.exhaust_out = 30.0f;
  in.fresh_in = 10.0f;
  in.fresh_out = 10.0f;
  correctTempWithCalibrationData(dest, in);
  TEST_ASSERT_EQUAL_FLOAT(45.0f, dest.exhaust_in);
  TEST_ASSERT_EQUAL_FLOAT(45.0f, dest.exhaust_out);
  TEST_ASSERT_EQUAL_FLOAT(45.0f, dest.fresh_in);
  TEST_ASSERT_EQUAL_FLOAT(40.0f, dest.fresh_out);

  calibration_sensor.fresh_out_temp.min = 30.0f;
  calibration_sensor.fresh_out_temp.max = 10.0f;
  calibration_sensor.ref_temp.min = 50.0f;
  calibration_sensor.ref_temp.max = 40.0f;
  in.fresh_out = 15.0f;
  correctTempWithCalibrationData(dest, in);
  TEST_ASSERT_EQUAL_FLOAT(42.5f, dest.fresh_out);
}

void test_correctHumidityWithCalibrationData()
{
  resetTest();
  calibration_sensor.fresh_out_humidity.min = 10.0f;
  calibration_sensor.fresh_out_humidity.max = 30.0f;
  calibration_sensor.exhaust_in_humidity.min = 20.0f;
  calibration_sensor.exhaust_in_humidity.max = 40.0f;
  calibration_sensor.ref_humidity.min = 40.0f;
  calibration_sensor.ref_humidity.max = 50.0f;
  fa_humidity_t dest, in;
  in.rel_fresh_out = 20.0f;
  in.rel_exhaust_in = 35.0f;
  correctHumidityWithCalibrationData(dest, in);
  TEST_ASSERT_EQUAL_FLOAT(45.0f, dest.rel_fresh_out);
  TEST_ASSERT_EQUAL_FLOAT(47.5f, dest.rel_exhaust_in);
}

void test_calcInterpolate()
{
  TEST_ASSERT_EQUAL_FLOAT(8, mapValue(-10, -10, 2, 8, 1));
  TEST_ASSERT_EQUAL_FLOAT(4.5, mapValue(-4, -10, 2, 8, 1));
  TEST_ASSERT_EQUAL_FLOAT(1, mapValue(-10, -10, 2, 1, 8));
  TEST_ASSERT_EQUAL_FLOAT(7.416666, mapValue(-9, -10, 2, 8, 1));
  TEST_ASSERT_EQUAL_FLOAT(7.416666, mapValue(1, -10, 2, 1, 8));
  TEST_ASSERT_EQUAL_FLOAT(1.583333, mapValue(-9, -10, 2, 1, 8));
  TEST_ASSERT_EQUAL_FLOAT(8, mapValue(10, 10, -2, 8, 1));
  TEST_ASSERT_EQUAL_FLOAT(1, mapValue(-2, 10, -2, 8, 1));
  TEST_ASSERT_EQUAL_FLOAT(8, mapValue(-2, 10, -2, -1, 8));

  TEST_ASSERT_TRUE(isInRange(0, 0, 5));
  TEST_ASSERT_TRUE(isInRange(1, 0, 5));
  TEST_ASSERT_TRUE(isInRange(1, 5, 1));
  TEST_ASSERT_FALSE(isInRange(5.1, 5, 1));
  TEST_ASSERT_FALSE(isInRange(5.1, 1, 5));
  TEST_ASSERT_TRUE(isInRange(1, -5, 1));
  TEST_ASSERT_FALSE(isInRange(1, -5, -1));
  TEST_ASSERT_TRUE(isInRange(-2, -3, 1));
  TEST_ASSERT_TRUE(isInRange(-3, -3, 1));
  TEST_ASSERT_TRUE(isInRange(-3, -5, -3));
  TEST_ASSERT_TRUE(isInRange(-3.1, -5, -3));
  TEST_ASSERT_FALSE(isInRange(-3.1, -3, 1));

  TEST_ASSERT_EQUAL_FLOAT(12, roundf(12.4));
  TEST_ASSERT_EQUAL_FLOAT(12, roundf(11.9));
  TEST_ASSERT_EQUAL_FLOAT(-12, roundf(-12.4));
  TEST_ASSERT_EQUAL_FLOAT(-12, roundf(-11.9));
}

void test_volumeCalibration()
{
  calibration_actuator.calibration_volume_liter = 60;
  calibration_actuator.fan_cal_time_main[0] = 12;
  calibration_actuator.fan_cal_time_main[5] = 27;
  TEST_ASSERT_EQUAL_FLOAT(18, getMainVolume(0));
  TEST_ASSERT_EQUAL_FLOAT(8, getMainVolume(5));
}


void test_calc_humidity_curve()
{
  resetTest();
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

void test_calc_frost_curve()
{
  resetTest();
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
