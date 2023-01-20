// Copyright (c) 2023 by Stefan Schmidt
#include "fa_sensors.h"
#include "fa_settings.h"
#include "fa_controller.h"
#include "fa_calc.h"
#include "fa_calibration.h"

static uint32_t sensors_now = 0;
bool s_flag_scan_sensors = false;


void sensorsUpdate()
{
  if (s_flag_scan_sensors)
  {
    sensorsScanIntern();
  }
  else if (intervalCheckSec(sensors_now, settings.temp_sensor_read_interval_sec))
  {
    sensorsRead();
    sensorsProcessValues();
  }
}

void sensorsScan()
{
  s_flag_scan_sensors = true;
}
