// Copyright (c) 2022 by Stefan Schmidt
#ifndef FA_SETTINGS_H
#define FA_SETTINGS_H

#include "fa_controller.h"

#define TEMP_SENSOR_READ_INTERVAL "temp_sensor_read_interval_sec"
#define CONTROLLER_INTERVAL "controller_interval_sec"
#define MEASUREMENT_ALPHA "measurement_alpha"
#define MANUAL_POWER_FAN_FRESH "manual_power_fan_fresh"
#define MANUAL_POWER_FAN_EXHAUST "manual_power_fan_exhaust"
#define MANUAL_POWER_FAN_FROST "manual_power_fan_frost"
#define MANUAL_FLAP_OPEN_FROST "manual_flap_open_frost"
#define CONTROLLER_MODE "controller_mode"


struct fa_settings_t
{
  uint16_t temp_sensor_read_interval_sec;
  uint16_t controller_interval_sec;
  controller_mode_t mode;
  float measurement_alpha;
  fa_actuator_t manual;
  bool changed;
};

extern fa_settings_t fa_settings;

void settings_load();
void settings_write();
void settings_clear();

#endif // FA_SETTINGS_H
