// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_STRUCTS_H
#define FA_STRUCTS_H

#include <stdint.h>

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

#endif // FA_STRUCTS_H
