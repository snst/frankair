// Copyright (c) 2023 by Stefan Schmidt
#include "fa_sensors.h"
#include "fa_settings.h"
#include "fa_controller.h"
#include "fa_calc.h"
#include "fa_calibration.h"
#include "fa_timer.h"
#include "fa_ota.h"

static FAInterval sensorInterval;
fa_override_t override = {0};

void sensorsSetup()
{
  sensorsSetupPlatform();
}

void sensorsUpdate()
{
  if (sensorInterval.checkSec(settings.temp_sensor_read_interval_sec))
  {
    if (!ota.downloading)
    {
      if (override.enabled)
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
}

void sensorsReadOverrideData()
{
  state_raw.temp.exhaust_in = override.temp.exhaust_in;
  state_raw.temp.fresh_out = override.temp.fresh_out;
  state_raw.temp.fresh_in = override.temp.fresh_in;
  state_raw.temp.exhaust_out = override.temp.exhaust_out;
  state_raw.humidity.rel_exhaust_in = override.humidity_rel_exhaust_in;
  state_raw.humidity.rel_fresh_out = override.humidity_rel_fresh_out;
}
