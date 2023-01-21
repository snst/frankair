#include <unity.h>
#include <stdio.h>
#include <iostream>
#include <cstring>

#include "fa_calc.h"
#include "fa_calibration.h"
#include "fa_settings.h"
#include "fa_common.h"

extern fa_state_t state;
extern fa_state_raw_t state_raw;

void cleanData();

void test_convertRelativeToAbsoluteHumidity(void)
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

void test_calcAverage(void)
{
  TEST_ASSERT_EQUAL_FLOAT(5.5f, calcAverage(3.3f, 7.7f));
  TEST_ASSERT_EQUAL_FLOAT(5.1f, calcAverage(3.3f, 7.7f, 1.5, 7.9));
}

void test_calcCorrectedValue(void)
{
  TEST_ASSERT_EQUAL_FLOAT(4.0f, calcCorrectedValue(1.75, 0.5, 2.5, 1.5, 5.5));
  TEST_ASSERT_EQUAL_FLOAT(2.5f, calcCorrectedValue(1, 0.5, 2.5, 1.5, 5.5));
}

void test_filterValue(void)
{
  cleanData();
  settings.measurement_alpha = 1.0f;
  float val = 10.0f;
  filterValue(val, 5.0f);
  TEST_ASSERT_EQUAL_FLOAT(5.0f, val);

  settings.measurement_alpha = 0.5f;
  val = 10.0f;
  filterValue(val, 5.0f);
  TEST_ASSERT_EQUAL_FLOAT(7.5f, val);
}

void test_correctTempWithCalibrationData(void)
{
  cleanData();
  fa_calibration_sensor.exhaust_in_temp.min = 10.0f;
  fa_calibration_sensor.exhaust_in_temp.max = 30.0f;
  fa_calibration_sensor.exhaust_out_temp.min = 20.0f;
  fa_calibration_sensor.exhaust_out_temp.max = 40.0f;
  fa_calibration_sensor.fresh_in_temp.min = -10.0f;
  fa_calibration_sensor.fresh_in_temp.max = 30.0f;
  fa_calibration_sensor.fresh_out_temp.min = 10.0f;
  fa_calibration_sensor.fresh_out_temp.max = 30.0f;
  fa_calibration_sensor.ref_temp.min = 40.0f;
  fa_calibration_sensor.ref_temp.max = 50.0f;
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

  fa_calibration_sensor.fresh_out_temp.min = 30.0f;
  fa_calibration_sensor.fresh_out_temp.max = 10.0f;
  fa_calibration_sensor.ref_temp.min = 50.0f;
  fa_calibration_sensor.ref_temp.max = 40.0f;
  in.fresh_out = 15.0f;
  correctTempWithCalibrationData(dest, in);
  TEST_ASSERT_EQUAL_FLOAT(42.5f, dest.fresh_out);
}

void test_correctHumidityWithCalibrationData(void)
{
  cleanData();
  fa_calibration_sensor.fresh_out_humidity.min = 10.0f;
  fa_calibration_sensor.fresh_out_humidity.max = 30.0f;
  fa_calibration_sensor.exhaust_in_humidity.min = 20.0f;
  fa_calibration_sensor.exhaust_in_humidity.max = 40.0f;
  fa_calibration_sensor.ref_humidity.min = 40.0f;
  fa_calibration_sensor.ref_humidity.max = 50.0f;
  fa_humidity_t dest, in;
  in.rel_fresh_out = 20.0f;
  in.rel_exhaust_in = 35.0f;
  correctHumidityWithCalibrationData(dest, in);
  TEST_ASSERT_EQUAL_FLOAT(45.0f, dest.rel_fresh_out);
  TEST_ASSERT_EQUAL_FLOAT(47.5f, dest.rel_exhaust_in);
}

void test_sensorsProcessValues(void)
{
  cleanData();
  fa_calibration_sensor.exhaust_in_temp.min = 10.0f;
  fa_calibration_sensor.exhaust_in_temp.max = 20.0f;
  fa_calibration_sensor.exhaust_out_temp.min = 10.0f;
  fa_calibration_sensor.exhaust_out_temp.max = 20.0f;
  fa_calibration_sensor.fresh_in_temp.min = 10.0f;
  fa_calibration_sensor.fresh_in_temp.max = 20.0f;
  fa_calibration_sensor.fresh_out_temp.min = 10.0f;
  fa_calibration_sensor.fresh_out_temp.max = 20.0f;
  fa_calibration_sensor.fresh_out_humidity.min = 10.0f;
  fa_calibration_sensor.fresh_out_humidity.max = 20.0f;
  fa_calibration_sensor.exhaust_in_humidity.min = 10.0f;
  fa_calibration_sensor.exhaust_in_humidity.max = 20.0f;
  fa_calibration_sensor.ref_temp.min = 20.0f;
  fa_calibration_sensor.ref_temp.max = 30.0f;
  fa_calibration_sensor.ref_humidity.min = 30.0f;
  fa_calibration_sensor.ref_humidity.max = 40.0f;

  state_raw.temp.exhaust_in = 11.0f;
  state_raw.temp.exhaust_out = 12.0f;
  state_raw.temp.fresh_in = 13.0f;
  state_raw.temp.fresh_out = 14.0f;
  state_raw.humidity.rel_exhaust_in = 15.0f;
  state_raw.humidity.rel_fresh_out = 16.0f;

  settings.use_calibration_temp = false;
  settings.use_calibration_humidity = false;
  settings.measurement_alpha = 0.5f;
  sensorsProcessValues();
  TEST_ASSERT_EQUAL_FLOAT(settings.measurement_alpha * state_raw.temp.exhaust_in, state.temp.exhaust_in);
  TEST_ASSERT_EQUAL_FLOAT(settings.measurement_alpha * state_raw.temp.exhaust_out, state.temp.exhaust_out);
  TEST_ASSERT_EQUAL_FLOAT(settings.measurement_alpha * state_raw.temp.fresh_in, state.temp.fresh_in);
  TEST_ASSERT_EQUAL_FLOAT(settings.measurement_alpha * state_raw.temp.fresh_out, state.temp.fresh_out);
  TEST_ASSERT_EQUAL_FLOAT(settings.measurement_alpha * state_raw.humidity.rel_exhaust_in, state.humidity.rel_exhaust_in);
  TEST_ASSERT_EQUAL_FLOAT(settings.measurement_alpha * state_raw.humidity.rel_fresh_out, state.humidity.rel_fresh_out);

  settings.use_calibration_temp = true;
  settings.use_calibration_humidity = true;
  settings.measurement_alpha = 1.0f;
  sensorsProcessValues();
  float offset = fa_calibration_sensor.ref_temp.min - fa_calibration_sensor.exhaust_in_temp.min;
  TEST_ASSERT_EQUAL_FLOAT(offset + settings.measurement_alpha * state_raw.temp.exhaust_in, state.temp.exhaust_in);
  TEST_ASSERT_EQUAL_FLOAT(offset + settings.measurement_alpha * state_raw.temp.exhaust_out, state.temp.exhaust_out);
  TEST_ASSERT_EQUAL_FLOAT(offset + settings.measurement_alpha * state_raw.temp.fresh_in, state.temp.fresh_in);
  TEST_ASSERT_EQUAL_FLOAT(offset + settings.measurement_alpha * state_raw.temp.fresh_out, state.temp.fresh_out);
  offset = fa_calibration_sensor.ref_humidity.min - fa_calibration_sensor.exhaust_in_humidity.min;
  TEST_ASSERT_EQUAL_FLOAT(offset + settings.measurement_alpha * state_raw.humidity.rel_exhaust_in, state.humidity.rel_exhaust_in);
  TEST_ASSERT_EQUAL_FLOAT(offset + settings.measurement_alpha * state_raw.humidity.rel_fresh_out, state.humidity.rel_fresh_out);
}

void test_volumeCalibration()
{
  fa_calibration_actuator.calibration_volume_liter = 60;
  fa_calibration_actuator.fan_cal_time_main[0] = 12;
  fa_calibration_actuator.fan_cal_time_main[5] = 27;
  TEST_ASSERT_EQUAL_FLOAT(18, getMainVolume(0));
  TEST_ASSERT_EQUAL_FLOAT(8, getMainVolume(5));
}

void test_duration(void)
{
  fa_duration_t d;
  resetDuration(d);
  addDurationMS(d, 500U);
  TEST_ASSERT_EQUAL(500U, d.ms);
  addDurationMS(d, 600U);
  TEST_ASSERT_EQUAL(100U, d.ms);
  TEST_ASSERT_EQUAL(1, d.sec);
}

void test_calcInterpolate(void)
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