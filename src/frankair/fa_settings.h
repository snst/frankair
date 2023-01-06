// Copyright (c) 2022 by Stefan Schmidt
#ifndef FA_SETTINGS_H
#define FA_SETTINGS_H

#include "fa_controller.h"

#define CFG_TEMP_SENSOR_READ_INTERVAL "cfg_temp_sensor_read_interval_sec"
#define CFG_CONTROLLER_INTERVAL "cfg_controller_interval_sec"
#define CFG_MEASUREMENT_ALPHA "cfg_measurement_alpha"
#define MANUAL_POWER_FAN_FRESH "manual_power_fan_fresh"
#define MANUAL_POWER_FAN_EXHAUST "manual_power_fan_exhaust"
#define MANUAL_POWER_FAN_FROST "manual_power_fan_frost"
#define MANUAL_FLAP_OPEN_FROST "manual_flap_open_frost"
#define CFG_CONTROLLER_MODE "cfg_controller_mode"
#define CFG_FLAP_MIN "cfg_flap_min"
#define CFG_FLAP_MAX "cfg_flap_max"


struct fa_settings_t
{
  uint16_t temp_sensor_read_interval_sec;
  uint16_t controller_interval_sec;
  uint8_t flap_min;
  uint8_t flap_max;
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
