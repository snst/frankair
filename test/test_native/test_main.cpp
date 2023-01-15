#include <unity.h>
#include <stdio.h>
#include <iostream>
#include <cstring>

#include "fa_calc.h"
#include "fa_calibration.h"
#include "fa_settings.h"

extern fa_state_t state;
extern fa_state_raw_t state_raw;

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
  memset(&state_raw, 0U, sizeof(state_raw));
  memset(&state, 0U, sizeof(state));
  memset(&settings, 0U, sizeof(settings));
}

void test_convertRelativeToAbsoluteHumidity(void);
void test_calcAverage(void);
void test_calcCorrectedValue(void);
void test_filterValue(void);
void test_correctTempWithCalibrationData(void);
void test_correctHumidityWithCalibrationData(void);
void test_sensorsProcessValues(void);
void test_controller_auto_update(void);
void test_controller_humidity_curve(void);
void test_controller_start_from_sniffing(void);
void test_controller_frost_curve(void);

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
  RUN_TEST(test_controller_auto_update);
  RUN_TEST(test_controller_humidity_curve);
  RUN_TEST(test_controller_start_from_sniffing);
  RUN_TEST(test_controller_frost_curve);

  UNITY_END();
  return 0;
}
