// Copyright (c) 2022 by Stefan Schmidt
#ifndef FA_SETTINGS_H
#define FA_SETTINGS_H

#define TEMP_SENSOR_READ_INTERVAL "temp_sensor_read_interval_sec"
#define CONTROLLER_INTERVAL "controller_interval_sec"
#define MEASUREMENT_ALPHA "measurement_alpha"

struct fa_settings_t
{
  uint16_t temp_sensor_read_interval_sec;
  uint16_t controller_interval_sec;
  float measurement_alpha;
};

extern fa_settings_t fa_settings;

void settings_load();
void settings_write();
void settings_clear();

#endif // FA_SETTINGS_H
