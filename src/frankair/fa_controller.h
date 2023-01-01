// Copyright (c) 2022 by Stefan Schmidt
#ifndef FA_CONTROLLER_H
#define FA_CONTROLLER_H

enum controller_mode_t { kOff = 0, kAuto, kManual, kUndefined };

struct fa_actuator_t
{
  uint8_t power_fan_fresh;
  uint8_t power_fan_exhaust;
  uint8_t power_fan_frost;
  uint8_t flap_open_frost;
};

struct fa_state_t
{
  fa_actuator_t actuator;
  controller_mode_t mode;
  float temp_fresh_in;
  float temp_fresh_out;
  float temp_exhaust_in;
  float temp_exhaust_out;
  float humidity_rel_exaust_in;
  float humidity_rel_fresh_out;
  float humidity_abs_exaust_in;
  float humidity_abs_fresh_out;
  float humidity_abs_delta;
  float effectiveness;
  float volume_fresh;
  float volume_exhaust;
  float liter;
};

extern fa_state_t fa_state;

void controller_setup();
void controller_update();

#endif // FA_CONTROLLER_H
