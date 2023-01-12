// Copyright (c) 2023 by Stefan Schmidt
#include "fa_sensors.h"
#include "fa_settings.h"
#include "fa_controller.h"
#include "fa_calc.h"
#include "fa_calibration.h"

static uint32_t sensors_now = 0;
bool s_flag_scan_sensors = false;


void sensors_update()
{
  if (s_flag_scan_sensors)
  {
    sensors_scan_intern();
  }
  else if (interval(sensors_now, fa_settings.temp_sensor_read_interval_sec))
  {
    sensorsRead();
    sensorsProcessValues();
  }
}

void sensors_scan()
{
  s_flag_scan_sensors = true;
}
