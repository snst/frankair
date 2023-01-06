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
  fa_settings.manual.power_fan_fresh = preferences.getUChar("D", 0);
  fa_settings.manual.power_fan_exhaust = preferences.getUChar("E", 0);
  fa_settings.manual.power_fan_frost = preferences.getUChar("F", 0);
  fa_settings.manual.flap_open_frost = preferences.getUChar("G", 0);
  fa_settings.mode = (controller_mode_t)preferences.getUChar("H", 0);
  fa_settings.flap_max = preferences.getUChar("I", 55);
  fa_settings.flap_min = preferences.getUChar("J", 150);
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
  preferences.putUChar("D", fa_settings.manual.power_fan_fresh);
  preferences.putUChar("E", fa_settings.manual.power_fan_exhaust);
  preferences.putUChar("F", fa_settings.manual.power_fan_frost);
  preferences.putUChar("G", fa_settings.manual.flap_open_frost);
  preferences.putUChar("H", fa_settings.mode);
  preferences.putUChar("I", fa_settings.flap_max);
  preferences.putUChar("J", fa_settings.flap_min);
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
  IMSG(CFG_TEMP_SENSOR_READ_INTERVAL, fa_settings.temp_sensor_read_interval_sec);
  IMSG(CFG_CONTROLLER_INTERVAL, fa_settings.controller_interval_sec);
  IMSG(CFG_MEASUREMENT_ALPHA, fa_settings.measurement_alpha);
}
