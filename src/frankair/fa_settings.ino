// Copyright (c) 2022 by Stefan Schmidt
#include "fa_settings.h"
#include <Preferences.h>
//#include <nvs_flash.h>

#define SETTINGS_NAMESPACE "settings"

fa_settings_t fa_settings;

void settings_load()
{
  Preferences preferences;
  preferences.begin(SETTINGS_NAMESPACE, true);
  fa_settings.temp_sensor_read_interval_sec = preferences.getUShort("A", 3000);
  fa_settings.controller_interval_sec = preferences.getUShort("B", 60000);
  fa_settings.measurement_alpha = preferences.getFloat("C", 1.0);
  preferences.end();
  settings_dump("Loading settings from flash..\n");
}

void settings_write()
{
  Preferences preferences;
  settings_dump("Writing settings to flash..\n");
  preferences.begin(SETTINGS_NAMESPACE, false);
  preferences.putUShort("A", fa_settings.temp_sensor_read_interval_sec);
  preferences.putUShort("B", fa_settings.controller_interval_sec);
  preferences.putFloat("C", fa_settings.measurement_alpha);
  preferences.end();
}

void settings_clear()
{
  Preferences preferences;
  IMSG("Clear settings in flash..\n");
  preferences.begin(SETTINGS_NAMESPACE, false);
  preferences.clear();
  preferences.end();
  /*nvs_flash_erase(); // erase the NVS partition and...
  nvs_flash_init(); // initialize the NVS partition.
  while(true);*/
}

void settings_dump(const char *txt)
{
  IMSG(txt);
  IMSG(TEMP_SENSOR_READ_INTERVAL, fa_settings.temp_sensor_read_interval_sec);
  IMSG(CONTROLLER_INTERVAL, fa_settings.controller_interval_sec);
  IMSG(MEASUREMENT_ALPHA, fa_settings.measurement_alpha);
}
