// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_SETTINGS_H
#define FA_SETTINGS_H

#include "fa_structs.h"

struct fa_settings_t
{
  uint16_t temp_sensor_read_interval_sec;
  uint16_t controller_interval_sec;
  controller_mode_t mode;
  float measurement_alpha;
  bool use_calibration;
  fa_actuator_t manual;
  uint8_t log_mask;
};

extern fa_settings_t fa_settings;

void calibration_load();
void calibration_write();
void calibration_clear();

void settings_load();
void settings_write();
void settings_clear();

void format();

#endif // FA_SETTINGS_H
