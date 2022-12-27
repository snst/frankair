// Copyright (c) 2022 by Stefan Schmidt
#include "fa_secrets.h"
#include "fa_thing.h"
#include "fa_log.h"
#include "fa_settings.h"
#include "fa_controller.h"

ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

void thing_setup()
{
  thing.add_wifi(SSID, SSID_PASSWORD);

  thing["cmd_reboot"] = []()
  {
    IMSG("cmd_reboot\n");
    ESP.restart();
  };

  thing["cmd_save_settings_to_flash"] = []()
  {
    IMSG("cmd_save_settings_to_flash\n");
    settings_write();
  };

  thing["cmd_load_settings_from_flash"] = []()
  {
    IMSG("cmd_load_settings_from_flash\n");
    settings_load();
  };

  thing["cmd_clear_settings_in_flash"] = []()
  {
    IMSG("cmd_clear_settings_in_flash\n");
    settings_clear();
  };

  thing["cmd_scan_sensors"] = []()
  {
    IMSG("cmd_scan_sensors\n");
    sensors_scan();
  };

  thing["settings"] << [](pson &in)
  {
    if (in.is_empty())
    {
      IMSG("Dashboard reads settings:\n");
      in[TEMP_SENSOR_READ_INTERVAL] = fa_settings.temp_sensor_read_interval_sec;
      in[CONTROLLER_INTERVAL] = fa_settings.controller_interval_sec;
      in[MEASUREMENT_ALPHA] = fa_settings.measurement_alpha;
    }
    else
    {
      IMSG("Dashboard writes settings:\n");
      fa_settings.temp_sensor_read_interval_sec = in[TEMP_SENSOR_READ_INTERVAL];
      fa_settings.controller_interval_sec = in[CONTROLLER_INTERVAL];
      fa_settings.measurement_alpha = in[MEASUREMENT_ALPHA];
    }

    IMSG(TEMP_SENSOR_READ_INTERVAL, fa_settings.temp_sensor_read_interval_sec);
    IMSG(CONTROLLER_INTERVAL, fa_settings.controller_interval_sec);
    IMSG(MEASUREMENT_ALPHA, fa_settings.measurement_alpha);
  };

  thing["controller"] >> [](pson &out)
  {
    out["temp_fresh_in"] = fa_controller.temp_fresh_in;
    out["temp_fresh_out"] = fa_controller.temp_fresh_out;
    out["temp_exhaust_in"] = fa_controller.temp_exhaust_in;
    out["temp_exhaust_out"] = fa_controller.temp_exhaust_out;
  };
}
