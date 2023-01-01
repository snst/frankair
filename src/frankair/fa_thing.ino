// Copyright (c) 2022 by Stefan Schmidt
#include "fa_secrets.h"
#include "fa_thing.h"
#include "fa_log.h"
#include "fa_settings.h"
#include "fa_controller.h"
#include "fa_common.h"

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
      in[CONTROLLER_MODE] = (uint8_t) fa_settings.mode;
      in[MANUAL_POWER_FAN_FRESH] = fa_settings.manual.power_fan_fresh;
      in[MANUAL_POWER_FAN_EXHAUST] = fa_settings.manual.power_fan_exhaust;
      in[MANUAL_POWER_FAN_FROST] = fa_settings.manual.power_fan_frost;
      in[MANUAL_FLAP_OPEN_FROST] = fa_settings.manual.flap_open_frost;
    }
    else
    {
      IMSG("Dashboard writes settings:\n");
      fa_settings.temp_sensor_read_interval_sec = in[TEMP_SENSOR_READ_INTERVAL];
      fa_settings.controller_interval_sec = in[CONTROLLER_INTERVAL];
      fa_settings.measurement_alpha = in[MEASUREMENT_ALPHA];
      fa_settings.mode = (controller_mode_t) (uint8_t) in[CONTROLLER_MODE];

      fa_settings.manual.power_fan_fresh = in[MANUAL_POWER_FAN_FRESH];
      fa_settings.manual.power_fan_exhaust = in[MANUAL_POWER_FAN_EXHAUST];
      fa_settings.manual.power_fan_frost = in[MANUAL_POWER_FAN_FROST];
      fa_settings.manual.flap_open_frost = in[MANUAL_FLAP_OPEN_FROST];
      fa_settings.changed = true;
    }

    IMSG(TEMP_SENSOR_READ_INTERVAL, fa_settings.temp_sensor_read_interval_sec);
    IMSG(CONTROLLER_INTERVAL, fa_settings.controller_interval_sec);
    IMSG(MEASUREMENT_ALPHA, fa_settings.measurement_alpha);
  };

  thing["controller"] >> [](pson &out)
  {
    out["temp_fresh_in"] = fa_state.temp_fresh_in;
    out["temp_fresh_out"] = fa_state.temp_fresh_out;
    out["temp_exhaust_in"] = fa_state.temp_exhaust_in;
    out["temp_exhaust_out"] = fa_state.temp_exhaust_out;
    out["mode"] = (uint8_t)fa_settings.mode;
    out["humidity_rel_exaust_in"] = fa_state.humidity_rel_exaust_in;
    out["humidity_rel_fresh_out"] = fa_state.humidity_rel_fresh_out;
    out["humidity_abs_exaust_in"] = fa_state.humidity_abs_exaust_in;
    out["humidity_abs_fresh_out"] = fa_state.humidity_abs_fresh_out;
    out["humidity_abs_delta"] = fa_state.humidity_abs_delta;
    out["effectiveness"] = fa_state.effectiveness;
    out["fan_power_fresh"] = fa_state.actuator.power_fan_fresh;
    out["fan_power_exhaust"] = fa_state.actuator.power_fan_exhaust;
    out["fan_power_frost"] = fa_state.actuator.power_fan_frost;
    out["flap_open_frost"] = fa_state.actuator.flap_open_frost;
    out["hours"] = now() / (1000.0*60*60);
    out["volume_fresh"] = fa_state.volume_fresh;
    out["volume_exhaust"] = fa_state.volume_exhaust;
    out["liter"] = fa_state.liter;
  };


}
