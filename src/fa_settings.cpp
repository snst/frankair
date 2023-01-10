// Copyright (c) 2023 by Stefan Schmidt
#include "fa_settings.h"
#include <Preferences.h>
#include "fa_log.h"
#include "fa_calibration.h"
//#include <nvs_flash.h>

const char *SETTINGS_NAMESPACE = "settings";
const char *CALIBRATION_NAMESPACE = "calibration";

fa_settings_t fa_settings;

void settings_load()
{
  Preferences preferences;
  preferences.begin(SETTINGS_NAMESPACE, true /*readonly*/);
  memset(&fa_settings, 0U, sizeof(fa_settings));
  size_t len = preferences.getBytes("s", &fa_settings, sizeof(fa_settings));
  IMSG(LM_SETTING, "Loading settings from flash..", len);
  if (0U == len)
  {
    fa_settings.temp_sensor_read_interval_sec = 1000U;
    fa_settings.controller_interval_sec = 5000U;
    fa_settings.measurement_alpha = 1.0f;
    fa_settings.mode = controller_mode_t::kManual;
  }
  preferences.end();
}

void settings_write()
{
  Preferences preferences;
  preferences.begin(SETTINGS_NAMESPACE, false /*readonly*/);
  size_t len = preferences.putBytes("s", &fa_settings, sizeof(fa_settings));
  IMSG(LM_SETTING, "Writing settings to flash..", len);
  preferences.end();
}

void settings_clear()
{
  Preferences preferences;
  IMSG(LM_SETTING, "Clear settings in flash..");
  preferences.begin(SETTINGS_NAMESPACE, false /*readonly*/);
  preferences.clear();
  preferences.end();
}

void calibration_load()
{
  memset(&fa_calibration_sensor, 0U, sizeof(fa_calibration_sensor));
  memset(&fa_calibration_actuator, 0U, sizeof(fa_calibration_actuator));
  Preferences preferences;
  preferences.begin(CALIBRATION_NAMESPACE, true /*readonly*/);
  size_t len = preferences.getBytes("s", &fa_calibration_sensor, sizeof(fa_calibration_sensor));
  IMSG(LM_SETTING, "Loading sensor calibration from flash..", len);
  len = preferences.getBytes("a", &fa_calibration_actuator, sizeof(fa_calibration_actuator));
  IMSG(LM_SETTING, "Loading actuator calibration from flash..", len);
  if (0U == len)
  {
    fa_calibration_actuator.flap_pos.min = 150U;
    fa_calibration_actuator.flap_pos.max = 55U;
    for (uint8_t i = 0U; i < FAN_POWER_STEPS; i++)
    {
      fa_calibration_actuator.fan_power_frost[i] = 255U * i / (FAN_POWER_STEPS - 1U);
      fa_calibration_actuator.fan_power_main[i] = fa_calibration_actuator.fan_power_frost[i];
    }
  }
  preferences.end();
}

void calibration_write()
{
  Preferences preferences;
  preferences.begin(CALIBRATION_NAMESPACE, false /*readonly*/);
  size_t len = preferences.putBytes("s", &fa_calibration_sensor, sizeof(fa_calibration_sensor));
  IMSG(LM_SETTING, "Writing sensor calibration to flash..", len);
  len = preferences.putBytes("a", &fa_calibration_actuator, sizeof(fa_calibration_actuator));
  IMSG(LM_SETTING, "Writing actuator calibration to flash..", len);
  preferences.end();
}

void calibration_clear()
{
  Preferences preferences;
  IMSG(LM_SETTING, "Clear calibration in flash..");
  preferences.begin(CALIBRATION_NAMESPACE, false /*readonly*/);
  preferences.clear();
  preferences.end();
}

void format()
{
  /*nvs_flash_erase(); // erase the NVS partition and...
  nvs_flash_init(); // initialize the NVS partition.
  while(true);*/
}