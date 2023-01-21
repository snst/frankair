// Copyright (c) 2023 by Stefan Schmidt
#include "fa_sensors.h"
#include "fa_settings.h"
#include "fa_controller.h"
#include "fa_calc.h"
#include "fa_calibration.h"

static uint32_t sensors_now = 0;
fa_override_t fa_override_sensors = {0};

void sensorsSetup()
{
  sensorsTargetSetup();
}

void sensorsUpdate()
{
  if (intervalCheckSec(sensors_now, settings.temp_sensor_read_interval_sec))
  {
    if (fa_override_sensors.enabled)
    {
      sensorsReadOverrideData();
    }
    else
    {
      sensorsRead();
    }
    sensorsProcessValues();
  }
}

void sensorsReadOverrideData()
{
  state_raw.temp.exhaust_in = fa_override_sensors.temp.exhaust_in;
  state_raw.temp.fresh_out = fa_override_sensors.temp.fresh_out;
  state_raw.temp.fresh_in = fa_override_sensors.temp.fresh_in;
  state_raw.temp.exhaust_out = fa_override_sensors.temp.exhaust_out;
  state_raw.humidity.rel_exhaust_in = fa_override_sensors.humidity_rel_exhaust_in;
  state_raw.humidity.rel_fresh_out = fa_override_sensors.humidity_rel_fresh_out;
}