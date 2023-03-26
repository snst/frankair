// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_STRUCTS_H
#define FA_STRUCTS_H

#include <stdint.h>
#include "fa_defines.h"

enum class controller_mode_t : uint8_t
{
  kOff = 0,
  kAuto,
  kManual,
  kUndefined
};

struct fa_duration_t
{
  uint32_t sec;
  uint32_t ms;
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
  float rel_exhaust_in;
  float rel_fresh_out;
  float abs_exhaust_in;
  float abs_fresh_out;
  float abs_delta;
};

struct fa_ctrl_active_t
{
  uint8_t humidity_fan_curve;
  uint8_t temp_fan_curve;
  uint8_t decisions;
  uint8_t frost_fan_curve;
};

struct fa_statistic_t
{
  float volume_m3_sum;
  float liter_sum;
  float volume_m3_per_hour;
  float liter_per_hour;
  fa_duration_t running;
};

struct fa_override_t
{
  fa_temp_t temp;
  float humidity_rel_exhaust_in;
  float humidity_rel_fresh_out;
  bool enabled;
};

struct fa_state_t
{
  uint8_t mode;         // controller_mode_t
  uint8_t sm_state;     // fa_sm_gen_StateId
  bool is_online;
  float efficiency;
  fa_actuator_t actuator;
  fa_temp_t temp;
  fa_humidity_t humidity;
  fa_ctrl_active_t ctrl_active;
  fa_duration_t running;
  uint8_t errors;
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

struct fa_fan_curve_t
{
  fa_val_level_pair_t item[FAN_CURVE_POINTS_MAX];
  uint8_t enabled_points;
  bool enabled;
};

struct fa_simple_ctrl_t
{
  float abs_hum_min_on;
  float abs_hum_min_wait;
  uint8_t rel_min_wait;
  uint8_t desired_temp_min;
  uint8_t desired_temp_max;
  bool rel_min_enabled;
  bool abs_min_enabled;
  bool desired_temp_min_enabled;
  bool desired_temp_max_enabled;
};

struct fa_frost_flap_ctrl_t
{
  float temp_min_open;
  float temp_min_close;
  uint8_t level_open;
  bool enabled;
};

struct fa_ctrl_t
{
  uint8_t fan_level_max;
  uint8_t fan_level_min;
  uint8_t fan_frost_level_max;
  uint8_t fan_frost_level_min;
  fa_fan_curve_t temp_fan_curve;
  fa_fan_curve_t humidity_fan_curve;
  fa_fan_curve_t frost_fan_curve;
  fa_simple_ctrl_t simple;
  fa_frost_flap_ctrl_t frost_flap_ctrl;
  int8_t fan_offset_fresh;
  int8_t fan_offset_exhaust;
};

struct fa_sniff_t
{
  uint16_t wait_sec;
  uint8_t sniff_sec;
  uint8_t fan_level_sniff;
  uint8_t fan_level_wait;
  bool enabled;
};

struct fa_settings_t
{
  uint8_t temp_sensor_read_interval_sec;
  uint8_t controller_interval_sec;
  uint8_t mode; // controller_mode_t
  uint8_t log_mask;
  float measurement_alpha_sniff;
  bool use_calibration_temp;
  bool use_calibration_humidity;
  bool use_calibrated_temp_for_abs_humidity;
  fa_actuator_t manual;
  fa_ctrl_t ctrl;
  fa_sniff_t sniff;
  uint8_t stream_interval_min;
  float measurement_alpha_on;
};

extern fa_state_t state;
extern fa_state_raw_t state_raw;
extern fa_settings_t settings;
extern fa_override_t override;

#endif // FA_STRUCTS_H
