// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_STRUCTS_H
#define FA_STRUCTS_H

#include <stdint.h>
#include "fa_defines.h"

enum controller_mode_t
{
  kOff = 0,
  kAuto,
  kManual,
  kUndefined
};

struct fa_actuator_t
{
  uint8_t power_fan_fresh;
  uint8_t power_fan_exhaust;
  uint8_t power_fan_frost;
  uint8_t flap_open_frost;
};

struct fa_temp_t
{
  float fresh_in;
  float fresh_out;
  float exhaust_in;
  float exhaust_out;
};

struct fa_humidity_t
{
  float rel_exaust_in;
  float rel_fresh_out;
  float abs_exaust_in;
  float abs_fresh_out;
  float abs_delta;
};

struct fa_state_t
{
  fa_actuator_t actuator;
  controller_mode_t mode;
  fa_temp_t temp;
  fa_humidity_t humidity;

  float effectiveness;
  float volume_fresh;
  float volume_exhaust;
  float liter;
  bool is_online;
};

struct fa_state_raw_t
{
  fa_temp_t temp;
  fa_humidity_t humidity;
};

struct fa_fan_level_constraint_t
{
  float val;
  uint8_t level;
};

struct fa_humidity_constraint_t
{
  float abs_min_start;
  float abs_min_stop;
  uint8_t rel_min_start;
};

struct fa_constraints_t
{
  fa_fan_level_constraint_t temp_fan[MAX_FAN_CONSTRAINTS];
  fa_fan_level_constraint_t humidity_fan[MAX_FAN_CONSTRAINTS];
  fa_humidity_constraint_t humidity;
  bool temp_fan_on;
  bool humidity_fan_on;
  bool humidity_on;
  uint8_t fan_level_max;
  uint8_t fan_level_min;
};


struct fa_sniff_t
{
  uint8_t interval_min;
  uint8_t duration_sec;
  uint8_t fan_level;
  bool on;
};

struct fa_settings_t
{
  uint16_t temp_sensor_read_interval_sec;
  uint16_t controller_interval_sec;
  controller_mode_t mode;
  float measurement_alpha;
  bool use_calibration;
  fa_actuator_t manual;
  uint8_t log_mask;
  fa_constraints_t constraints;
  fa_sniff_t sniff;
};

#endif // FA_STRUCTS_H
