// Copyright (c) 2023 by Stefan Schmidt

#include "fa_thing.h"
#include "fa_thing_priv.h"

#include "fa_secrets.h"
#include "fa_log.h"
#include "fa_settings.h"
#include "fa_controller.h"
#include "fa_common.h"
#include "fa_sensors.h"
#include "fa_calibration.h"

const char *CMD_REBOOT = "cmd_reboot";
const char *CMD_SAVE_SETTINGS = "cmd_save_settings";
const char *CMD_LOAD_SETTINGS = "cmd_load_settings";
const char *CMD_CLEAR_SETTINGS = "cmd_clear_settings";
const char *CMD_SAVE_CALIBRATION = "cmd_save_calibration";
const char *CMD_LOAD_CALIBRATION = "cmd_load_calibration";
const char *CMD_CLEAR_CALIBRATION = "cmd_clear_calibration";
const char *CMD_SCAN_SENSORS = "cmd_scan_sensors";
const char *CMD_CALIBRATE_TEMP_LOW = "cmd_calibrate_temp_low";
const char *CMD_CALIBRATE_TEMP_HIGH = "cmd_calibrate_temp_high";
const char *CMD_CALIBRATE_HUMIDITY_LOW = "cmd_calibrate_humidity_low";
const char *CMD_CALIBRATE_HUMIDIRY_HIGH = "cmd_calibrate_humidity_high";

const char *CFG_TEMP_SENSOR_READ_INTERVAL = "cfg_temp_sensor_read_interval_sec";
const char *CFG_CONTROLLER_INTERVAL = "cfg_controller_interval_sec";
const char *CFG_MEASUREMENT_ALPHA = "cfg_measurement_alpha";
const char *MANUAL_POWER_FAN_FRESH = "manual_power_fan_fresh";
const char *MANUAL_POWER_FAN_EXHAUST = "manual_power_fan_exhaust";
const char *MANUAL_POWER_FAN_FROST = "manual_power_fan_frost";
const char *MANUAL_FLAP_OPEN_FROST = "manual_flap_open_frost";
const char *CFG_CONTROLLER_MODE = "cfg_controller_mode";
const char *CFG_USE_CALIBRATION = "cfg_use_calibration";
const char *CFG_LOG_MASK = "cfg_log_mask";
const char *CAL_FLAP_POS_MIN = "cal_flap_pos_min";
const char *CAL_FLAP_POS_MAX = "cal_flap_pos_max";
const char *CAL_TEMP_EXHAST_IN_MIN = "cal_temp_exhaust_in_min";
const char *CAL_TEMP_EXHAST_IN_MAX = "cal_temp_exhaust_in_max";
const char *CAL_TEMP_EXHAST_OUT_MIN = "cal_temp_exhaust_out_min";
const char *CAL_TEMP_EXHAST_OUT_MAX = "cal_temp_exhaust_out_max";
const char *CAL_TEMP_FRESH_IN_MIN = "cal_temp_fresh_in_min";
const char *CAL_TEMP_FRESH_IN_MAX = "cal_temp_fresh_in_max";
const char *CAL_TEMP_FRESH_OUT_MIN = "cal_temp_fresh_out_min";
const char *CAL_TEMP_FRESH_OUT_MAX = "cal_temp_fresh_out_max";
const char *CAL_HUMIDITY_EXHAST_IN_MIN = "cal_humidity_exhaust_in_min";
const char *CAL_HUMIDITY_EXHAST_IN_MAX = "cal_humidity_exhaust_in_max";
const char *CAL_HUMIDITY_FRESH_OUT_MIN = "cal_humidity_fresh_out_min";
const char *CAL_HUMIDITY_FRESH_OUT_MAX = "cal_humidity_fresh_out_max";

ThingerESP32 thing(FA_USERNAME, FA_DEVICE_ID, FA_DEVICE_CREDENTIAL);

void thing_update()
{
  thing.handle();
  fa_state.is_online = thing.is_connected();
}

void thing_setup()
{
  thing.add_wifi(FA_SSID, FA_SSID_PASSWORD);

  ADD_CMD(CMD_REBOOT, ESP.restart)
  ADD_CMD(CMD_SAVE_SETTINGS, settings_write)
  ADD_CMD(CMD_LOAD_SETTINGS, settings_load)
  ADD_CMD(CMD_CLEAR_SETTINGS, settings_clear)
  ADD_CMD(CMD_SAVE_CALIBRATION, calibration_write)
  ADD_CMD(CMD_LOAD_CALIBRATION, calibration_load)
  ADD_CMD(CMD_CLEAR_CALIBRATION, calibration_clear)
  ADD_CMD(CMD_SCAN_SENSORS, sensors_scan)
  ADD_CMD(CMD_CALIBRATE_TEMP_LOW, sensors_calibrate_temp_low)
  ADD_CMD(CMD_CALIBRATE_TEMP_HIGH, sensors_calibrate_temp_high)
  ADD_CMD(CMD_CALIBRATE_HUMIDITY_LOW, sensors_calibrate_humidity_low)
  ADD_CMD(CMD_CALIBRATE_HUMIDIRY_HIGH, sensors_calibrate_humidity_high)

  thing["settings"] << [](pson &in)
  {
    if (in.is_empty())
    {
      IMSG(LM_THING, "Dashboard reads settings..");
      in[CFG_TEMP_SENSOR_READ_INTERVAL] = fa_settings.temp_sensor_read_interval_sec;
      in[CFG_CONTROLLER_INTERVAL] = fa_settings.controller_interval_sec;
      in[CFG_MEASUREMENT_ALPHA] = fa_settings.measurement_alpha;
      in[CFG_CONTROLLER_MODE] = (uint8_t)fa_settings.mode;
      in[CFG_USE_CALIBRATION] = fa_settings.use_calibration;
      in[CFG_LOG_MASK] = fa_settings.log_mask;
      in[MANUAL_POWER_FAN_FRESH] = fa_settings.manual.power_fan_fresh;
      in[MANUAL_POWER_FAN_EXHAUST] = fa_settings.manual.power_fan_exhaust;
      in[MANUAL_POWER_FAN_FROST] = fa_settings.manual.power_fan_frost;
      in[MANUAL_FLAP_OPEN_FROST] = fa_settings.manual.flap_open_frost;
    }
    else
    {
      IMSG(LM_THING, "Dashboard writes settings..");
      fa_settings.temp_sensor_read_interval_sec = in[CFG_TEMP_SENSOR_READ_INTERVAL];
      fa_settings.controller_interval_sec = in[CFG_CONTROLLER_INTERVAL];
      fa_settings.measurement_alpha = in[CFG_MEASUREMENT_ALPHA];
      fa_settings.mode = (controller_mode_t)(uint8_t)in[CFG_CONTROLLER_MODE];
      fa_settings.use_calibration = in[CFG_USE_CALIBRATION];
      fa_settings.log_mask = in[CFG_LOG_MASK];
      fa_settings.manual.power_fan_fresh = in[MANUAL_POWER_FAN_FRESH];
      fa_settings.manual.power_fan_exhaust = in[MANUAL_POWER_FAN_EXHAUST];
      fa_settings.manual.power_fan_frost = in[MANUAL_POWER_FAN_FROST];
      fa_settings.manual.flap_open_frost = in[MANUAL_FLAP_OPEN_FROST];
      g_force_update = true;
    }
  };

  thing["calibration_sensor"] << [](pson &in)
  {
    if (in.is_empty())
    {
      IMSG(LM_THING, "Dashboard reads sensor calibration..");
      in[CAL_TEMP_EXHAST_IN_MIN] = fa_calibration_sensor.exhaust_in_temp.min;
      in[CAL_TEMP_EXHAST_IN_MAX] = fa_calibration_sensor.exhaust_in_temp.max;
      in[CAL_TEMP_EXHAST_OUT_MIN] = fa_calibration_sensor.exhaust_out_temp.min;
      in[CAL_TEMP_EXHAST_OUT_MAX] = fa_calibration_sensor.exhaust_out_temp.max;
      in[CAL_TEMP_FRESH_IN_MIN] = fa_calibration_sensor.fresh_in_temp.min;
      in[CAL_TEMP_FRESH_IN_MAX] = fa_calibration_sensor.fresh_in_temp.max;
      in[CAL_TEMP_FRESH_OUT_MIN] = fa_calibration_sensor.fresh_out_temp.min;
      in[CAL_TEMP_FRESH_OUT_MAX] = fa_calibration_sensor.fresh_out_temp.max;
      in[CAL_HUMIDITY_EXHAST_IN_MIN] = fa_calibration_sensor.exhaust_in_humidity.min;
      in[CAL_HUMIDITY_EXHAST_IN_MAX] = fa_calibration_sensor.exhaust_in_humidity.max;
      in[CAL_HUMIDITY_FRESH_OUT_MIN] = fa_calibration_sensor.fresh_out_humidity.min;
      in[CAL_HUMIDITY_FRESH_OUT_MAX] = fa_calibration_sensor.fresh_out_humidity.max;
    }
    else
    {
      IMSG(LM_THING, "Dashboard writes sensor calibration..");
      fa_calibration_sensor.exhaust_in_temp.min = in[CAL_TEMP_EXHAST_IN_MIN];
      fa_calibration_sensor.exhaust_in_temp.max = in[CAL_TEMP_EXHAST_IN_MAX];
      fa_calibration_sensor.exhaust_out_temp.min = in[CAL_TEMP_EXHAST_OUT_MIN];
      fa_calibration_sensor.exhaust_out_temp.max = in[CAL_TEMP_EXHAST_OUT_MAX];
      fa_calibration_sensor.fresh_in_temp.min = in[CAL_TEMP_FRESH_IN_MIN];
      fa_calibration_sensor.fresh_in_temp.max = in[CAL_TEMP_FRESH_IN_MAX];
      fa_calibration_sensor.fresh_out_temp.min = in[CAL_TEMP_FRESH_OUT_MIN];
      fa_calibration_sensor.fresh_out_temp.max = in[CAL_TEMP_FRESH_OUT_MAX];
      fa_calibration_sensor.exhaust_in_humidity.min = in[CAL_HUMIDITY_EXHAST_IN_MIN];
      fa_calibration_sensor.exhaust_in_humidity.max = in[CAL_HUMIDITY_EXHAST_IN_MAX];
      fa_calibration_sensor.fresh_out_humidity.min = in[CAL_HUMIDITY_FRESH_OUT_MIN];
      fa_calibration_sensor.fresh_out_humidity.max = in[CAL_HUMIDITY_FRESH_OUT_MAX];
      g_force_update = true;
    }
  };

  thing["calibration_actuator"] << [](pson &in)
  {
    if (in.is_empty())
    {
      IMSG(LM_THING, "Dashboard reads actuator calibration..");
      in[CAL_FLAP_POS_MIN] = fa_calibration_actuator.flap_pos.min;
      in[CAL_FLAP_POS_MAX] = fa_calibration_actuator.flap_pos.max;
      for (uint8_t i = 0U; i < FAN_POWER_STEPS; i++)
      {
        char str[20];
        sprintf(str, "cal_fan_main_%u", i);
        in[(const char *)str] = fa_calibration_actuator.fan_power_main[i];
        sprintf(str, "cal_fan_frost_%u", i);
        in[(const char *)str] = fa_calibration_actuator.fan_power_frost[i];
      }
    }
    else
    {
      IMSG(LM_THING, "Dashboard writes actuator calibration..");
      fa_calibration_actuator.flap_pos.min = in[CAL_FLAP_POS_MIN];
      fa_calibration_actuator.flap_pos.max = in[CAL_FLAP_POS_MAX];
      for (uint8_t i = 0U; i < FAN_POWER_STEPS; i++)
      {
        char str[20];
        sprintf(str, "cal_fan_main_%u", i);
        fa_calibration_actuator.fan_power_main[i] = in[(const char *)str];
        sprintf(str, "cal_fan_frost_%u", i);
        fa_calibration_actuator.fan_power_frost[i] = in[(const char *)str];
      }
      g_force_update = true;
    }
  };

  thing["controller"] >> [](pson &out)
  {
    out["temp_fresh_in"] = fa_state.temp.fresh_in;
    out["temp_fresh_out"] = fa_state.temp.fresh_out;
    out["temp_exhaust_in"] = fa_state.temp.exhaust_in;
    out["temp_exhaust_out"] = fa_state.temp.exhaust_out;
    out["mode"] = (uint8_t)fa_settings.mode;
    out["humidity_rel_exaust_in"] = fa_state.humidity.rel_exaust_in;
    out["humidity_rel_fresh_out"] = fa_state.humidity.rel_fresh_out;
    out["humidity_abs_exaust_in"] = fa_state.humidity.abs_exaust_in;
    out["humidity_abs_fresh_out"] = fa_state.humidity.abs_fresh_out;
    out["humidity_abs_delta"] = fa_state.humidity.abs_delta;
    out["effectiveness"] = fa_state.effectiveness;
    out["fan_power_fresh"] = fa_state.actuator.power_fan_fresh;
    out["fan_power_exhaust"] = fa_state.actuator.power_fan_exhaust;
    out["fan_power_frost"] = fa_state.actuator.power_fan_frost;
    out["flap_open_frost"] = fa_state.actuator.flap_open_frost;
    out["hours"] = now() / (1000.0 * 60 * 60);
    out["volume_fresh"] = fa_state.volume_fresh;
    out["volume_exhaust"] = fa_state.volume_exhaust;
    out["liter"] = fa_state.liter;
    out["use_calibration"] = fa_settings.use_calibration;
  };

  thing["state_raw"] >> [](pson &out)
  {
    out["temp_fresh_in"] = fa_state_raw.temp.fresh_in;
    out["temp_fresh_out"] = fa_state_raw.temp.fresh_out;
    out["temp_exhaust_in"] = fa_state_raw.temp.exhaust_in;
    out["temp_exhaust_out"] = fa_state_raw.temp.exhaust_out;
    out["humidity_rel_exaust_in"] = fa_state_raw.humidity.rel_exaust_in;
    out["humidity_rel_fresh_out"] = fa_state_raw.humidity.rel_fresh_out;
  };
}
