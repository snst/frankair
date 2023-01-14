// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_STRUCTS_H
#define FA_STRUCTS_H

#include <stdint.h>
#include "fa_defines.h"
enum class controller_mode_t
{
  kOff = 0,
  kAuto,
  kManual,
  kUndefined
};

enum class controller_submode_auto_t
{
  kWait = 0,
  kOn,
  kSniff,
  kUndefined
};

struct fa_actuator_t
{
  uint8_t level_fan_fresh;
  uint8_t level_fan_exhaust;
  uint8_t level_fan_frost;
  uint8_t open_flap_frost;
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

struct fa_ctrl_active_t
{
  bool humidity_fan_level;
  bool temp_fan_level;
  bool humidity_fan_off;
};

struct fa_state_t
{
  fa_actuator_t actuator;
  controller_mode_t mode;
  controller_submode_auto_t submode_auto;

  fa_temp_t temp;
  fa_humidity_t humidity;
  fa_ctrl_active_t ctrl_active;

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

struct fa_val_level_pair_t
{
  float val;
  uint8_t level;
};

struct fa_fan_level_ctrl_t
{
  fa_val_level_pair_t item[MAX_FAN_CONSTRAINTS];
  bool enabled;
};

struct fa_humidity_ctrl_t
{
  float abs_min_start;
  float abs_min_stop;
  uint8_t rel_min_start;
  bool enabled;
};

struct fa_ctrl_t
{
  fa_fan_level_ctrl_t temp_fan_level;
  fa_fan_level_ctrl_t humidity_fan_level;
  fa_humidity_ctrl_t humidity_fan_off;
  uint8_t fan_level_max;
  uint8_t fan_level_min;
};

struct fa_sniff_t
{
  uint16_t interval_sec;
  uint8_t duration_sec;
  uint8_t fan_level;
  bool enabled;
};

struct fa_settings_t
{
  uint8_t temp_sensor_read_interval_sec;
  uint8_t controller_interval_sec;
  controller_mode_t mode;
  float measurement_alpha;
  bool use_calibration;
  fa_actuator_t manual;
  uint8_t log_mask;
  fa_ctrl_t ctrl;
  fa_sniff_t sniff;
};

#endif // FA_STRUCTS_H
