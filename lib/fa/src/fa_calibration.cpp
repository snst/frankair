// Copyright (c) 2023 by Stefan Schmidt
#include "fa_calibration.h"
#include "fa_structs.h"
#include "fa_calc.h"

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
    fa_calibration_sensor.exhaust_in_humidity.min = state_raw.humidity.rel_exaust_in;
    fa_calibration_sensor.fresh_out_humidity.min = state_raw.humidity.rel_fresh_out;
    fa_calibration_sensor.ref_humidity.min = calcAverage(fa_calibration_sensor.exhaust_in_humidity.min,
                                                         fa_calibration_sensor.fresh_out_humidity.min);
}

void sensorsCalibrateHumidityHigh()
{
    fa_calibration_sensor.exhaust_in_humidity.max = state_raw.humidity.rel_exaust_in;
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
    dest.rel_exaust_in = calcCorrectedValue(in.rel_exaust_in,
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
