// Copyright (c) 2023 by Stefan Schmidt
#include "test_helper.h"

void setUp(void)
{
  // set stuff up here
}

void tearDown(void)
{
  // clean stuff up here
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
  RUN_TEST(test_calcInterpolate);
  RUN_TEST(test_volumeCalibration);
  RUN_TEST(test_calc_humidity_curve);
  RUN_TEST(test_calc_frost_curve);
  RUN_TEST(test_smActionModeOnAdjustFrost_on);
  RUN_TEST(test_smActionModeOnAdjustFrost_off);
  RUN_TEST(test_smEnterInitialState);

  UNITY_END();
  return 0;
}
