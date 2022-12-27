// Copyright (c) 2022 by Stefan Schmidt
#ifndef FA_CONTROLLER_H
#define FA_CONTROLLER_H

struct fa_controller_t
{
  float temp_fresh_in;
  float temp_fresh_out;
  float temp_exhaust_in;
  float temp_exhaust_out;
};

extern fa_controller_t fa_controller;

void controller_setup();
void controller_update();

#endif // FA_CONTROLLER_H
