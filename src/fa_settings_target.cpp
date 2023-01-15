// Copyright (c) 2023 by Stefan Schmidt
#include "fa_settings.h"
#include <Preferences.h>
#include "fa_log.h"
#include "fa_calibration.h"
//#include <nvs_flash.h>

const char *SETTINGS_NAMESPACE = "settings";
const char *CALIBRATION_NAMESPACE = "calibration";

fa_settings_t settings;

void settingsLoad()
{
  Preferences preferences;
  preferences.begin(SETTINGS_NAMESPACE, true /*readonly*/);
  memset(&settings, 0U, sizeof(settings));
  size_t len = preferences.getBytes("s", &settings, sizeof(settings));
  IMSG(LM_SETTING, "Loading settings from flash..", len);
  if (0U == len)
  {
    settings.temp_sensor_read_interval_sec = 1U;
    settings.controller_interval_sec = 5U;
    settings.measurement_alpha = 1.0f;
    settings.mode = (uint8_t)controller_mode_t::kManual;
    settings.sniff.duration_sec = 60U;
    settings.sniff.interval_sec = (30U * 60U);
    settings.sniff.fan_level = 5U;
    settings.ctrl.fan_level_max = FAN_LEVEL_MAX;
    settings.ctrl.fan_level_min = FAN_LEVEL_MIN;
    settings.ctrl.humidity_fan_off.abs_min_start = 1.0f;
    settings.ctrl.humidity_fan_off.abs_min_stop = 0.5;
    settings.ctrl.humidity_fan_off.rel_min_start = 50U;
    settings.ctrl.temp_fan_level.item[0].val = 5.0f;
    settings.ctrl.temp_fan_level.item[0].level = 3U;
    settings.ctrl.temp_fan_level.item[1].val = 10.0f;
    settings.ctrl.temp_fan_level.item[1].level = 5U;
    settings.ctrl.temp_fan_level.item[2].val = 15.0f;
    settings.ctrl.temp_fan_level.item[2].level = 8U;
    settings.ctrl.humidity_fan_level.item[0].val = 1.0f;
    settings.ctrl.humidity_fan_level.item[0].level = 4U;
    settings.ctrl.humidity_fan_level.item[1].val = 2.0f;
    settings.ctrl.humidity_fan_level.item[1].level = 6U;
    settings.ctrl.humidity_fan_level.item[2].val = 3.0f;
    settings.ctrl.humidity_fan_level.item[2].level = 8;
  }
  preferences.end();
}

void settingsWrite()
{
  Preferences preferences;
  preferences.begin(SETTINGS_NAMESPACE, false /*readonly*/);
  size_t len = preferences.putBytes("s", &settings, sizeof(settings));
  IMSG(LM_SETTING, "Writing settings to flash..", len);
  preferences.end();
}

void settingsClear()
{
  Preferences preferences;
  IMSG(LM_SETTING, "Clear settings in flash..");
  preferences.begin(SETTINGS_NAMESPACE, false /*readonly*/);
  preferences.clear();
  preferences.end();
}

void calibrationLoad()
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
    for (uint8_t i = 0U; i < FAN_LEVEL_STEPS; i++)
    {
      fa_calibration_actuator.fan_pwm_frost[i] = 255U * i / (FAN_LEVEL_STEPS - 1U);
      fa_calibration_actuator.fan_pwm_main[i] = fa_calibration_actuator.fan_pwm_frost[i];
    }
  }
  preferences.end();
}

void calibrationWrite()
{
  Preferences preferences;
  preferences.begin(CALIBRATION_NAMESPACE, false /*readonly*/);
  size_t len = preferences.putBytes("s", &fa_calibration_sensor, sizeof(fa_calibration_sensor));
  IMSG(LM_SETTING, "Writing sensor calibration to flash..", len);
  len = preferences.putBytes("a", &fa_calibration_actuator, sizeof(fa_calibration_actuator));
  IMSG(LM_SETTING, "Writing actuator calibration to flash..", len);
  preferences.end();
}

void calibrationClear()
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