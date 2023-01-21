// Copyright (c) 2023 by Stefan Schmidt
#include "fa_calibration.h"
#include "fa_structs.h"
#include "fa_calc.h"
#include "fa_common.h"
#include <cstring>

fa_calibration_sensor_t calibration_sensor;
fa_calibration_actuator_t calibration_actuator;
extern fa_state_raw_t state_raw;

void sensorsCalibrateTempLow()
{
    calibration_sensor.exhaust_in_temp.min = state_raw.temp.exhaust_in;
    calibration_sensor.exhaust_out_temp.min = state_raw.temp.exhaust_out;
    calibration_sensor.fresh_in_temp.min = state_raw.temp.fresh_in;
    calibration_sensor.fresh_out_temp.min = state_raw.temp.fresh_out;
    calibration_sensor.ref_temp.min = calcAverage(calibration_sensor.exhaust_in_temp.min,
                                                     calibration_sensor.exhaust_out_temp.min,
                                                     calibration_sensor.fresh_in_temp.min,
                                                     calibration_sensor.fresh_out_temp.min);
}

void sensorsCalibrateTempHigh()
{
    calibration_sensor.exhaust_in_temp.max = state_raw.temp.exhaust_in;
    calibration_sensor.exhaust_out_temp.max = state_raw.temp.exhaust_out;
    calibration_sensor.fresh_in_temp.max = state_raw.temp.fresh_in;
    calibration_sensor.fresh_out_temp.max = state_raw.temp.fresh_out;
    calibration_sensor.ref_temp.max = calcAverage(calibration_sensor.exhaust_in_temp.max,
                                                     calibration_sensor.exhaust_out_temp.max,
                                                     calibration_sensor.fresh_in_temp.max,
                                                     calibration_sensor.fresh_out_temp.max);
}

void sensorsCalibrateHumidityLow()
{
    calibration_sensor.exhaust_in_humidity.min = state_raw.humidity.rel_exhaust_in;
    calibration_sensor.fresh_out_humidity.min = state_raw.humidity.rel_fresh_out;
    calibration_sensor.ref_humidity.min = calcAverage(calibration_sensor.exhaust_in_humidity.min,
                                                         calibration_sensor.fresh_out_humidity.min);
}

void sensorsCalibrateHumidityHigh()
{
    calibration_sensor.exhaust_in_humidity.max = state_raw.humidity.rel_exhaust_in;
    calibration_sensor.fresh_out_humidity.max = state_raw.humidity.rel_fresh_out;
    calibration_sensor.ref_humidity.max = calcAverage(calibration_sensor.exhaust_in_humidity.max,
                                                         calibration_sensor.fresh_out_humidity.max);
}

void correctTempWithCalibrationData(fa_temp_t &dest, fa_temp_t &in)
{
    dest.exhaust_in = calcCorrectedValue(in.exhaust_in,
                                         calibration_sensor.exhaust_in_temp.min,
                                         calibration_sensor.exhaust_in_temp.max,
                                         calibration_sensor.ref_temp.min,
                                         calibration_sensor.ref_temp.max);

    dest.exhaust_out = calcCorrectedValue(in.exhaust_out,
                                          calibration_sensor.exhaust_out_temp.min,
                                          calibration_sensor.exhaust_out_temp.max,
                                          calibration_sensor.ref_temp.min,
                                          calibration_sensor.ref_temp.max);

    dest.fresh_in = calcCorrectedValue(in.fresh_in,
                                       calibration_sensor.fresh_in_temp.min,
                                       calibration_sensor.fresh_in_temp.max,
                                       calibration_sensor.ref_temp.min,
                                       calibration_sensor.ref_temp.max);

    dest.fresh_out = calcCorrectedValue(in.fresh_out,
                                        calibration_sensor.fresh_out_temp.min,
                                        calibration_sensor.fresh_out_temp.max,
                                        calibration_sensor.ref_temp.min,
                                        calibration_sensor.ref_temp.max);
}

void correctHumidityWithCalibrationData(fa_humidity_t &dest, fa_humidity_t &in)
{
    dest.rel_exhaust_in = calcCorrectedValue(in.rel_exhaust_in,
                                            calibration_sensor.exhaust_in_humidity.min,
                                            calibration_sensor.exhaust_in_humidity.max,
                                            calibration_sensor.ref_humidity.min,
                                            calibration_sensor.ref_humidity.max);
    dest.rel_fresh_out = calcCorrectedValue(in.rel_fresh_out,
                                            calibration_sensor.fresh_out_humidity.min,
                                            calibration_sensor.fresh_out_humidity.max,
                                            calibration_sensor.ref_humidity.min,
                                            calibration_sensor.ref_humidity.max);
}

float getMainVolume(uint8_t level)
{
    level = adjustFanLevelToValidRange(level);
    uint8_t sec = calibration_actuator.fan_cal_time_main[level];
    float m3perHour = 0.0f;
    if (sec > 0U)
    {
        m3perHour = ((float)calibration_actuator.calibration_volume_liter) * 3.6f / sec;
    }
    return m3perHour;
}

void calibrationDefault()
{
    IMSG(LINFO, "calibrationDefault()");
    memset(&calibration_actuator, 0U, sizeof(calibration_actuator));
    calibration_actuator.flap_pos.min = 150U;
    calibration_actuator.flap_pos.max = 55U;
    calibration_actuator.calibration_volume_liter = 60U;
    calibration_actuator.fan_pwm_main[0] = 0U;
    calibration_actuator.fan_pwm_main[1] = 85U;
    calibration_actuator.fan_pwm_main[2] = 130U;
    calibration_actuator.fan_pwm_main[3] = 160U;
    calibration_actuator.fan_pwm_main[4] = 180U;
    calibration_actuator.fan_pwm_main[5] = 200U;
    calibration_actuator.fan_pwm_main[6] = 210U;
    calibration_actuator.fan_pwm_main[7] = 220U;
    calibration_actuator.fan_pwm_main[8] = 240U;
    calibration_actuator.fan_pwm_main[9] = 245U;
    calibration_actuator.fan_pwm_main[10] = 255U;
    calibration_actuator.fan_cal_time_main[0] = 0U;
    calibration_actuator.fan_cal_time_main[1] = 240U;
    calibration_actuator.fan_cal_time_main[2] = 120U;
    calibration_actuator.fan_cal_time_main[3] = 50U;
    calibration_actuator.fan_cal_time_main[4] = 38U;
    calibration_actuator.fan_cal_time_main[5] = 27U;
    calibration_actuator.fan_cal_time_main[6] = 23U;
    calibration_actuator.fan_cal_time_main[7] = 19U;
    calibration_actuator.fan_cal_time_main[8] = 16U;
    calibration_actuator.fan_cal_time_main[9] = 14U;
    calibration_actuator.fan_cal_time_main[10] = 12U;
    calibration_actuator.fan_pwm_frost[0] = 0U;
    calibration_actuator.fan_pwm_frost[1] = 55U;
    calibration_actuator.fan_pwm_frost[2] = 75U;
    calibration_actuator.fan_pwm_frost[3] = 90U;
    calibration_actuator.fan_pwm_frost[4] = 105U;
    calibration_actuator.fan_pwm_frost[5] = 125U;
    calibration_actuator.fan_pwm_frost[6] = 145U;
    calibration_actuator.fan_pwm_frost[7] = 165U;
    calibration_actuator.fan_pwm_frost[8] = 190U;
    calibration_actuator.fan_pwm_frost[9] = 219U;
    calibration_actuator.fan_pwm_frost[10] = 255U;
}
