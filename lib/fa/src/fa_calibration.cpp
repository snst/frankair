// Copyright (c) 2023 by Stefan Schmidt
#include "fa_calibration.h"
#include "fa_structs.h"
#include "fa_calc.h"
#include "fa_common.h"
#include <cstring>

fa_calibration_sensor_t fa_calibration_sensor;
fa_calibration_actuator_t fa_calibration_actuator;
extern fa_state_raw_t state_raw;

void sensorsCalibrateTempLow()
{
    fa_calibration_sensor.exhaust_in_temp.min = state_raw.temp.exhaust_in;
    fa_calibration_sensor.exhaust_out_temp.min = state_raw.temp.exhaust_out;
    fa_calibration_sensor.fresh_in_temp.min = state_raw.temp.fresh_in;
    fa_calibration_sensor.fresh_out_temp.min = state_raw.temp.fresh_out;
    fa_calibration_sensor.ref_temp.min = calcAverage(fa_calibration_sensor.exhaust_in_temp.min,
                                                     fa_calibration_sensor.exhaust_out_temp.min,
                                                     fa_calibration_sensor.fresh_in_temp.min,
                                                     fa_calibration_sensor.fresh_out_temp.min);
}

void sensorsCalibrateTempHigh()
{
    fa_calibration_sensor.exhaust_in_temp.max = state_raw.temp.exhaust_in;
    fa_calibration_sensor.exhaust_out_temp.max = state_raw.temp.exhaust_out;
    fa_calibration_sensor.fresh_in_temp.max = state_raw.temp.fresh_in;
    fa_calibration_sensor.fresh_out_temp.max = state_raw.temp.fresh_out;
    fa_calibration_sensor.ref_temp.max = calcAverage(fa_calibration_sensor.exhaust_in_temp.max,
                                                     fa_calibration_sensor.exhaust_out_temp.max,
                                                     fa_calibration_sensor.fresh_in_temp.max,
                                                     fa_calibration_sensor.fresh_out_temp.max);
}

void sensorsCalibrateHumidityLow()
{
    fa_calibration_sensor.exhaust_in_humidity.min = state_raw.humidity.rel_exhaust_in;
    fa_calibration_sensor.fresh_out_humidity.min = state_raw.humidity.rel_fresh_out;
    fa_calibration_sensor.ref_humidity.min = calcAverage(fa_calibration_sensor.exhaust_in_humidity.min,
                                                         fa_calibration_sensor.fresh_out_humidity.min);
}

void sensorsCalibrateHumidityHigh()
{
    fa_calibration_sensor.exhaust_in_humidity.max = state_raw.humidity.rel_exhaust_in;
    fa_calibration_sensor.fresh_out_humidity.max = state_raw.humidity.rel_fresh_out;
    fa_calibration_sensor.ref_humidity.max = calcAverage(fa_calibration_sensor.exhaust_in_humidity.max,
                                                         fa_calibration_sensor.fresh_out_humidity.max);
}

void correctTempWithCalibrationData(fa_temp_t &dest, fa_temp_t &in)
{
    dest.exhaust_in = calcCorrectedValue(in.exhaust_in,
                                         fa_calibration_sensor.exhaust_in_temp.min,
                                         fa_calibration_sensor.exhaust_in_temp.max,
                                         fa_calibration_sensor.ref_temp.min,
                                         fa_calibration_sensor.ref_temp.max);

    dest.exhaust_out = calcCorrectedValue(in.exhaust_out,
                                          fa_calibration_sensor.exhaust_out_temp.min,
                                          fa_calibration_sensor.exhaust_out_temp.max,
                                          fa_calibration_sensor.ref_temp.min,
                                          fa_calibration_sensor.ref_temp.max);

    dest.fresh_in = calcCorrectedValue(in.fresh_in,
                                       fa_calibration_sensor.fresh_in_temp.min,
                                       fa_calibration_sensor.fresh_in_temp.max,
                                       fa_calibration_sensor.ref_temp.min,
                                       fa_calibration_sensor.ref_temp.max);

    dest.fresh_out = calcCorrectedValue(in.fresh_out,
                                        fa_calibration_sensor.fresh_out_temp.min,
                                        fa_calibration_sensor.fresh_out_temp.max,
                                        fa_calibration_sensor.ref_temp.min,
                                        fa_calibration_sensor.ref_temp.max);
}

void correctHumidityWithCalibrationData(fa_humidity_t &dest, fa_humidity_t &in)
{
    dest.rel_exhaust_in = calcCorrectedValue(in.rel_exhaust_in,
                                            fa_calibration_sensor.exhaust_in_humidity.min,
                                            fa_calibration_sensor.exhaust_in_humidity.max,
                                            fa_calibration_sensor.ref_humidity.min,
                                            fa_calibration_sensor.ref_humidity.max);
    dest.rel_fresh_out = calcCorrectedValue(in.rel_fresh_out,
                                            fa_calibration_sensor.fresh_out_humidity.min,
                                            fa_calibration_sensor.fresh_out_humidity.max,
                                            fa_calibration_sensor.ref_humidity.min,
                                            fa_calibration_sensor.ref_humidity.max);
}

float getMainVolume(uint8_t level)
{
    level = adjustFanLevelToValidRange(level);
    uint8_t sec = fa_calibration_actuator.fan_cal_time_main[level];
    float m3perHour = 0.0f;
    if (sec > 0U)
    {
        m3perHour = ((float)fa_calibration_actuator.calibration_volume_liter) * 3.6f / sec;
    }
    return m3perHour;
}


void calibrationDefault()
{
    IMSG(LINFO, "calibrationDefault()");
    memset(&fa_calibration_actuator, 0U, sizeof(fa_calibration_actuator));
    fa_calibration_actuator.flap_pos.min = 150U;
    fa_calibration_actuator.flap_pos.max = 55U;
    fa_calibration_actuator.calibration_volume_liter = 60U;
    fa_calibration_actuator.fan_pwm_main[0] = 0U;
    fa_calibration_actuator.fan_pwm_main[1] = 85U;
    fa_calibration_actuator.fan_pwm_main[2] = 130U;
    fa_calibration_actuator.fan_pwm_main[3] = 160U;
    fa_calibration_actuator.fan_pwm_main[4] = 180U;
    fa_calibration_actuator.fan_pwm_main[5] = 200U;
    fa_calibration_actuator.fan_pwm_main[6] = 210U;
    fa_calibration_actuator.fan_pwm_main[7] = 220U;
    fa_calibration_actuator.fan_pwm_main[8] = 240U;
    fa_calibration_actuator.fan_pwm_main[9] = 245U;
    fa_calibration_actuator.fan_pwm_main[10] = 255U;
    fa_calibration_actuator.fan_cal_time_main[0] = 0U;
    fa_calibration_actuator.fan_cal_time_main[1] = 240U;
    fa_calibration_actuator.fan_cal_time_main[2] = 120U;
    fa_calibration_actuator.fan_cal_time_main[3] = 50U;
    fa_calibration_actuator.fan_cal_time_main[4] = 38U;
    fa_calibration_actuator.fan_cal_time_main[5] = 27U;
    fa_calibration_actuator.fan_cal_time_main[6] = 23U;
    fa_calibration_actuator.fan_cal_time_main[7] = 19U;
    fa_calibration_actuator.fan_cal_time_main[8] = 16U;
    fa_calibration_actuator.fan_cal_time_main[9] = 14U;
    fa_calibration_actuator.fan_cal_time_main[10] = 12U;
    fa_calibration_actuator.fan_pwm_frost[0] = 0U;
    fa_calibration_actuator.fan_pwm_frost[1] = 55U;
    fa_calibration_actuator.fan_pwm_frost[2] = 75U;
    fa_calibration_actuator.fan_pwm_frost[3] = 90U;
    fa_calibration_actuator.fan_pwm_frost[4] = 105U;
    fa_calibration_actuator.fan_pwm_frost[5] = 125U;
    fa_calibration_actuator.fan_pwm_frost[6] = 145U;
    fa_calibration_actuator.fan_pwm_frost[7] = 165U;
    fa_calibration_actuator.fan_pwm_frost[8] = 190U;
    fa_calibration_actuator.fan_pwm_frost[9] = 219U;
    fa_calibration_actuator.fan_pwm_frost[10] = 255U;
}