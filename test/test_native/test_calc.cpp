#include <unity.h>
#include <stdio.h>
#include <iostream>
#include <cstring>

#include "fa_calc.h"
#include "fa_calibration.h"
#include "fa_settings.h"

fa_settings_t fa_settings;
fa_state_t fa_state;
fa_state_raw_t fa_state_raw;

void setUp(void)
{
  // set stuff up here
}

void tearDown(void)
{
  // clean stuff up here
}

void cleanData()
{
  memset(&fa_calibration_sensor, 0U, sizeof(fa_calibration_sensor));
  memset(&fa_state_raw, 0U, sizeof(fa_state_raw));
  memset(&fa_state, 0U, sizeof(fa_state));
  memset(&fa_settings, 0U, sizeof(fa_settings));
}

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
  fa_settings.measurement_alpha = 1.0f;
  float val = 10.0f;
  filterValue("", val, 5.0f);
  TEST_ASSERT_EQUAL_FLOAT(5.0f, val);

  fa_settings.measurement_alpha = 0.5f;
  val = 10.0f;
  filterValue("", val, 5.0f);
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
  in.rel_exaust_in = 35.0f;
  correctHumidityWithCalibrationData(dest, in);
  TEST_ASSERT_EQUAL_FLOAT(45.0f, dest.rel_fresh_out);
  TEST_ASSERT_EQUAL_FLOAT(47.5f, dest.rel_exaust_in);
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

  fa_state_raw.temp.exhaust_in = 11.0f;
  fa_state_raw.temp.exhaust_out = 12.0f;
  fa_state_raw.temp.fresh_in = 13.0f;
  fa_state_raw.temp.fresh_out = 14.0f;
  fa_state_raw.humidity.rel_exaust_in = 15.0f;
  fa_state_raw.humidity.rel_fresh_out = 16.0f;

  fa_settings.use_calibration = false;
  fa_settings.measurement_alpha = 0.5f;
  sensorsProcessValues();
  TEST_ASSERT_EQUAL_FLOAT(fa_settings.measurement_alpha * fa_state_raw.temp.exhaust_in, fa_state.temp.exhaust_in);
  TEST_ASSERT_EQUAL_FLOAT(fa_settings.measurement_alpha * fa_state_raw.temp.exhaust_out, fa_state.temp.exhaust_out);
  TEST_ASSERT_EQUAL_FLOAT(fa_settings.measurement_alpha * fa_state_raw.temp.fresh_in, fa_state.temp.fresh_in);
  TEST_ASSERT_EQUAL_FLOAT(fa_settings.measurement_alpha * fa_state_raw.temp.fresh_out, fa_state.temp.fresh_out);
  TEST_ASSERT_EQUAL_FLOAT(fa_settings.measurement_alpha * fa_state_raw.humidity.rel_exaust_in, fa_state.humidity.rel_exaust_in);
  TEST_ASSERT_EQUAL_FLOAT(fa_settings.measurement_alpha * fa_state_raw.humidity.rel_fresh_out, fa_state.humidity.rel_fresh_out);

  fa_settings.use_calibration = true;
  fa_settings.measurement_alpha = 1.0f;
  sensorsProcessValues();
  float offset = fa_calibration_sensor.ref_temp.min - fa_calibration_sensor.exhaust_in_temp.min;
  TEST_ASSERT_EQUAL_FLOAT(offset + fa_settings.measurement_alpha * fa_state_raw.temp.exhaust_in, fa_state.temp.exhaust_in);
  TEST_ASSERT_EQUAL_FLOAT(offset + fa_settings.measurement_alpha * fa_state_raw.temp.exhaust_out, fa_state.temp.exhaust_out);
  TEST_ASSERT_EQUAL_FLOAT(offset + fa_settings.measurement_alpha * fa_state_raw.temp.fresh_in, fa_state.temp.fresh_in);
  TEST_ASSERT_EQUAL_FLOAT(offset + fa_settings.measurement_alpha * fa_state_raw.temp.fresh_out, fa_state.temp.fresh_out);
  offset = fa_calibration_sensor.ref_humidity.min - fa_calibration_sensor.exhaust_in_humidity.min;
  TEST_ASSERT_EQUAL_FLOAT(offset + fa_settings.measurement_alpha * fa_state_raw.humidity.rel_exaust_in, fa_state.humidity.rel_exaust_in);
  TEST_ASSERT_EQUAL_FLOAT(offset + fa_settings.measurement_alpha * fa_state_raw.humidity.rel_fresh_out, fa_state.humidity.rel_fresh_out);
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();

  RUN_TEST(test_convertRelativeToAbsoluteHumidity);
  RUN_TEST(test_calcAverage);
  RUN_TEST(test_calcCorrectedValue);
  RUN_TEST(test_filterValue);
  RUN_TEST(test_correctTempWithCalibrationData);
  RUN_TEST(test_correctHumidityWithCalibrationData);
  RUN_TEST(test_sensorsProcessValues);

  UNITY_END();
  return 0;
}
