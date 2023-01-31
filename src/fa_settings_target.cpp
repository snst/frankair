// Copyright (c) 2023 by Stefan Schmidt
#include "fa_settings.h"
#include <Preferences.h>
#include "fa_log.h"
#include "fa_calibration.h"
//#include <nvs_flash.h>

const char *SETTINGS_NAMESPACE = "settings";
const char *CALIBRATION_NAMESPACE = "calibration";

void settingsLoad()
{
  Preferences preferences;
  preferences.begin(SETTINGS_NAMESPACE, true /*readonly*/);
  memset(&settings, 0U, sizeof(settings));
  size_t len = preferences.getBytes("s", &settings, sizeof(settings));
  IMSG(LINFO, "Loading settings from flash..", len);
  if (0U == len)
  {
    settingsDefault();
  }
  preferences.end();
  settingsSanitize();
}

void settingsWrite()
{
  Preferences preferences;
  preferences.begin(SETTINGS_NAMESPACE, false /*readonly*/);
  size_t len = preferences.putBytes("s", &settings, sizeof(settings));
  IMSG(LINFO, "Writing settings to flash..", len);
  preferences.end();
}

void settingsClear()
{
  Preferences preferences;
  IMSG(LINFO, "Clear settings in flash..");
  preferences.begin(SETTINGS_NAMESPACE, false /*readonly*/);
  preferences.clear();
  preferences.end();
}

void calibrationLoad()
{
  memset(&calibration_sensor, 0U, sizeof(calibration_sensor));
  memset(&calibration_actuator, 0U, sizeof(calibration_actuator));
  Preferences preferences;
  preferences.begin(CALIBRATION_NAMESPACE, true /*readonly*/);
  size_t len = preferences.getBytes("s", &calibration_sensor, sizeof(calibration_sensor));
  IMSG(LINFO, "Loading sensor calibration from flash..", len);
  len = preferences.getBytes("a", &calibration_actuator, sizeof(calibration_actuator));
  IMSG(LINFO, "Loading actuator calibration from flash..", len);
  if (0U == len)
  {
    calibrationDefault();
  }
  preferences.end();
}

void calibrationWrite()
{
  Preferences preferences;
  preferences.begin(CALIBRATION_NAMESPACE, false /*readonly*/);
  size_t len = preferences.putBytes("s", &calibration_sensor, sizeof(calibration_sensor));
  IMSG(LINFO, "Writing sensor calibration to flash..", len);
  len = preferences.putBytes("a", &calibration_actuator, sizeof(calibration_actuator));
  IMSG(LINFO, "Writing actuator calibration to flash..", len);
  preferences.end();
}

void calibrationClear()
{
  Preferences preferences;
  IMSG(LINFO, "Clear calibration in flash..");
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
