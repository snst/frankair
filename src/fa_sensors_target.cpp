// Copyright (c) 2023 by Stefan Schmidt
#include "fa_sensors_dallas_target.h"
#include "fa_sensors_bme280_target.h"
#include "fa_controller.h"
#include "fa_calc.h"
#include <cstdio>

void sensorsSetup()
{
  sensorsDallasSetup();
  sensorsBME280Setup();
}

void sensorsRead()
{
  sensorsDallasRead();
  sensorsBME280Read();
  char buf[128];
  sprintf(buf, "RAW ei %f, fo %f, fi %f, eo %f, hi %f, ho %f",
          state_raw.temp.exhaust_in,
          state_raw.temp.fresh_out,
          state_raw.temp.fresh_in,
          state_raw.temp.exhaust_out,
          state_raw.humidity.rel_exaust_in,
          state_raw.humidity.rel_fresh_out);
  IMSG(LSENSOR, buf);
}

void sensorsScanIntern()
{
  sensorsDallasScan();
}
