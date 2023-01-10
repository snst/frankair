// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_CALIBRATION_H
#define FA_CALIBRATION_H

#include <stdint.h>
#include "fa_structs.h"

#define FAN_POWER_STEPS 11

struct fa_minMaxFloat_t
{
  float min;
  float max;
};

struct fa_minMaxUint8_t
{
  uint8_t min;
  uint8_t max;
};

struct fa_calibration_sensor_t
{
  fa_minMaxFloat_t exhaust_in_temp;
  fa_minMaxFloat_t exhaust_out_temp;
  fa_minMaxFloat_t fresh_in_temp;
  fa_minMaxFloat_t fresh_out_temp;
  fa_minMaxFloat_t exhaust_in_humidity;
  fa_minMaxFloat_t fresh_out_humidity;
  fa_minMaxFloat_t ref_temp;
  fa_minMaxFloat_t ref_humidity;
};

struct fa_calibration_actuator_t
{
  fa_minMaxUint8_t flap_pos;
  uint8_t fan_power_main[FAN_POWER_STEPS];
  uint8_t fan_power_frost[FAN_POWER_STEPS];
};

void sensors_calibrate_temp_low();
void sensors_calibrate_temp_high();
void sensors_calibrate_humidity_low();
void sensors_calibrate_humidity_high();

void correctTempWithCalibrationData(fa_temp_t &dest, fa_temp_t &in);
void correctHumidityWithCalibrationData(fa_humidity_t &dest, fa_humidity_t &in);

extern fa_calibration_sensor_t fa_calibration_sensor;
extern fa_calibration_actuator_t fa_calibration_actuator;

#endif // FA_CALIBRATION_H