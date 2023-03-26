// Copyright (c) 2023 by Stefan Schmidt
#ifndef TEST_HELPER_H
#define TEST_HELPER_H

#include <stdint.h>
#include <unity.h>
#include "fa_calibration.h"

#include "fa_calc.h"
#include "fa_calibration.h"
#include "fa_settings.h"
#include "fa_common.h"
#include "fa_structs.h"
#include "fa_sm.h"
#include "fa_controller.h"
#include "fa_fan.h"

extern fa_sm_gen g_state_machine;

void resetTest();

void test_convertRelativeToAbsoluteHumidity();
void test_calcAverage();
void test_calcCorrectedValue();
void test_filterValue();
void test_correctTempWithCalibrationData();
void test_correctHumidityWithCalibrationData();
void test_volumeCalibration();
void test_calcInterpolate();
void test_calc_humidity_curve();
void test_calc_frost_curve();
void test_smActionModeOnAdjustFrost_on();
void test_smActionModeOnAdjustFrost_off();
void test_smEnterInitialState();

#endif // TEST_HELPER_H
