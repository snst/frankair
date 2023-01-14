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
const char *MANUAL_LEVEL_FAN_FRESH = "manual_level_fan_fresh";
const char *MANUAL_LEVEL_FAN_EXHAUST = "manual_level_fan_exhaust";
const char *MANUAL_LEVEL_FAN_FROST = "manual_level_fan_frost";
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

const char *CFG_FAN_LEVEL_MAX = "cfg_fan_level_max";
const char *CFG_FAN_LEVEL_MIN = "cfg_fan_level_min";
const char *CFG_TEMP_FAN_LEVEL_ENABLED = "cfg_temp_fan_level_enabled";
const char *CFG_HUMIDITY_FAN_LEVEL_ENABLED = "cfg_humidity_fan_level_enabled";
const char *CFG_HUMIDITY_FAN_OFF_ENABLED = "cfg_humidity_fan_off_enabled";
const char *CFG_HUMIDITY_FAN_OFF_ABS_MIN_START = "cfg_humidity_off_abs_min_start";
const char *CFG_HUMIDITY_FAN_OFF_ABS_MIN_STOP = "cfg_humidity_off_abs_min_stop";
const char *CFG_HUMIDITY_FAN_OFF_REL_MIN_STOP = "cfg_humidity_off_rel_min_stop";
const char *CFG_SNIFF_INTERVAL_SEC = "cfg_sniff_interval_sec";
const char *CFG_SNIFF_DURATION_SEC = "cfg_sniff_duration_sec";
const char *CFG_SNIFF_FAN_LEVEL = "cfg_sniff_fan_level";
const char *CFG_SNIFF_ENABLED = "cfg_sniff_enabled";
const char *CFG_TEMP_0_VAL = "cfg_temp_0_val";
const char *CFG_TEMP_0_FAN = "cfg_temp_0_fan";
const char *CFG_TEMP_1_VAL = "cfg_temp_1_val";
const char *CFG_TEMP_1_FAN = "cfg_temp_1_fan";
const char *CFG_TEMP_2_VAL = "cfg_temp_2_val";
const char *CFG_TEMP_2_FAN = "cfg_temp_2_fan";
const char *CFG_HUMIDITY_0_VAL = "cfg_humidity_0_val";
const char *CFG_HUMIDITY_0_FAN = "cfg_humidity_0_fan";
const char *CFG_HUMIDITY_1_VAL = "cfg_humidity_1_val";
const char *CFG_HUMIDITY_1_FAN = "cfg_humidity_1_fan";
const char *CFG_HUMIDITY_2_VAL = "cfg_humidity_2_val";
const char *CFG_HUMIDITY_2_FAN = "cfg_humidity_2_fan";

ThingerESP32 thing(FA_USERNAME, FA_DEVICE_ID, FA_DEVICE_CREDENTIAL);

void thingUpdate()
{
  thing.handle();
  state.is_online = thing.is_connected();
}

void thingSetup()
{
  thing.add_wifi(FA_SSID, FA_SSID_PASSWORD);

  ADD_CMD(CMD_REBOOT, ESP.restart)
  ADD_CMD(CMD_SAVE_SETTINGS, settingsWrite)
  ADD_CMD(CMD_LOAD_SETTINGS, settingsLoad)
  ADD_CMD(CMD_CLEAR_SETTINGS, settingsClear)
  ADD_CMD(CMD_SAVE_CALIBRATION, calibrationWrite)
  ADD_CMD(CMD_LOAD_CALIBRATION, calibrationLoad)
  ADD_CMD(CMD_CLEAR_CALIBRATION, calibrationClear)
  ADD_CMD(CMD_SCAN_SENSORS, sensorsScan)
  ADD_CMD(CMD_CALIBRATE_TEMP_LOW, sensorsCalibrateTempLow)
  ADD_CMD(CMD_CALIBRATE_TEMP_HIGH, sensorsCalibrateTempHigh)
  ADD_CMD(CMD_CALIBRATE_HUMIDITY_LOW, sensorsCalibrateHumidityLow)
  ADD_CMD(CMD_CALIBRATE_HUMIDIRY_HIGH, sensorsCalibrateHumidityHigh)

  thing["settings"] << [](pson &in)
  {
    if (in.is_empty())
    {
      IMSG(LM_THING, "Dashboard reads settings..");
      in[CFG_TEMP_SENSOR_READ_INTERVAL] = settings.temp_sensor_read_interval_sec;
      in[CFG_CONTROLLER_INTERVAL] = settings.controller_interval_sec;
      in[CFG_MEASUREMENT_ALPHA] = settings.measurement_alpha;
      in[CFG_CONTROLLER_MODE] = (uint8_t)settings.mode;
      in[CFG_USE_CALIBRATION] = settings.use_calibration;
      in[CFG_LOG_MASK] = settings.log_mask;
      in[CFG_SNIFF_INTERVAL_SEC] = settings.sniff.interval_sec;
      in[CFG_SNIFF_DURATION_SEC] = settings.sniff.duration_sec;
      in[CFG_SNIFF_FAN_LEVEL] = settings.sniff.fan_level;
      in[CFG_SNIFF_ENABLED] = settings.sniff.enabled;
      in[MANUAL_LEVEL_FAN_FRESH] = settings.manual.level_fan_fresh;
      in[MANUAL_LEVEL_FAN_EXHAUST] = settings.manual.level_fan_exhaust;
      in[MANUAL_LEVEL_FAN_FROST] = settings.manual.level_fan_frost;
      in[MANUAL_FLAP_OPEN_FROST] = settings.manual.open_flap_frost;
      in[CFG_FAN_LEVEL_MAX] = settings.ctrl.fan_level_max;
      in[CFG_FAN_LEVEL_MIN] = settings.ctrl.fan_level_min;

    }
    else
    {
      IMSG(LM_THING, "Dashboard writes settings..");
      settings.temp_sensor_read_interval_sec = in[CFG_TEMP_SENSOR_READ_INTERVAL];
      settings.controller_interval_sec = in[CFG_CONTROLLER_INTERVAL];
      settings.measurement_alpha = in[CFG_MEASUREMENT_ALPHA];
      settings.mode = (controller_mode_t)(uint8_t)in[CFG_CONTROLLER_MODE];
      settings.use_calibration = in[CFG_USE_CALIBRATION];
      //settings.log_mask = in[CFG_LOG_MASK];
      settings.sniff.interval_sec = in[CFG_SNIFF_INTERVAL_SEC];
      settings.sniff.duration_sec = in[CFG_SNIFF_DURATION_SEC];
      settings.sniff.fan_level = in[CFG_SNIFF_FAN_LEVEL];
      settings.sniff.enabled = in[CFG_SNIFF_ENABLED];
      settings.manual.level_fan_fresh = in[MANUAL_LEVEL_FAN_FRESH];
      settings.manual.level_fan_exhaust = in[MANUAL_LEVEL_FAN_EXHAUST];
      settings.manual.level_fan_frost = in[MANUAL_LEVEL_FAN_FROST];
      settings.manual.open_flap_frost = in[MANUAL_FLAP_OPEN_FROST];
      settings.ctrl.fan_level_max = in[CFG_FAN_LEVEL_MAX];
      settings.ctrl.fan_level_min = in[CFG_FAN_LEVEL_MIN];
      force_update = true;
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
      force_update = true;
    }
  };

  thing["calibration_actuator"] << [](pson &in)
  {
    if (in.is_empty())
    {
      IMSG(LM_THING, "Dashboard reads actuator calibration..");
      in[CAL_FLAP_POS_MIN] = fa_calibration_actuator.flap_pos.min;
      in[CAL_FLAP_POS_MAX] = fa_calibration_actuator.flap_pos.max;
      for (uint8_t i = 0U; i < FAN_LEVEL_STEPS; i++)
      {
        char str[20];
        sprintf(str, "cal_fan_main_%u", i);
        in[(const char *)str] = fa_calibration_actuator.fan_pwm_main[i];
        sprintf(str, "cal_fan_frost_%u", i);
        in[(const char *)str] = fa_calibration_actuator.fan_pwm_frost[i];
      }
    }
    else
    {
      IMSG(LM_THING, "Dashboard writes actuator calibration..");
      fa_calibration_actuator.flap_pos.min = in[CAL_FLAP_POS_MIN];
      fa_calibration_actuator.flap_pos.max = in[CAL_FLAP_POS_MAX];
      for (uint8_t i = 0U; i < FAN_LEVEL_STEPS; i++)
      {
        char str[20];
        sprintf(str, "cal_fan_main_%u", i);
        fa_calibration_actuator.fan_pwm_main[i] = in[(const char *)str];
        sprintf(str, "cal_fan_frost_%u", i);
        fa_calibration_actuator.fan_pwm_frost[i] = in[(const char *)str];
      }
      force_update = true;
    }
  };

  thing["ctrl"] << [](pson &in)
  {
    if (in.is_empty())
    {
      IMSG(LM_THING, "Dashboard reads ctrl..");
      in[CFG_TEMP_FAN_LEVEL_ENABLED] = settings.ctrl.temp_fan_level.enabled;
      in[CFG_HUMIDITY_FAN_LEVEL_ENABLED] = settings.ctrl.humidity_fan_level.enabled;
      in[CFG_HUMIDITY_FAN_OFF_ENABLED] = settings.ctrl.humidity_fan_off.enabled;
      in[CFG_HUMIDITY_FAN_OFF_ABS_MIN_START] = settings.ctrl.humidity_fan_off.abs_min_start;
      in[CFG_HUMIDITY_FAN_OFF_ABS_MIN_STOP] = settings.ctrl.humidity_fan_off.abs_min_stop;
      in[CFG_HUMIDITY_FAN_OFF_REL_MIN_STOP] = settings.ctrl.humidity_fan_off.rel_min_start;
      in[CFG_TEMP_0_VAL] = settings.ctrl.temp_fan_level.item[0].val;
      in[CFG_TEMP_0_FAN] = settings.ctrl.temp_fan_level.item[0].level;
      in[CFG_TEMP_1_VAL] = settings.ctrl.temp_fan_level.item[1].val;
      in[CFG_TEMP_1_FAN] = settings.ctrl.temp_fan_level.item[1].level;
      in[CFG_TEMP_2_VAL] = settings.ctrl.temp_fan_level.item[2].val;
      in[CFG_TEMP_2_FAN] = settings.ctrl.temp_fan_level.item[2].level;
      in[CFG_HUMIDITY_0_VAL] = settings.ctrl.humidity_fan_level.item[0].val;
      in[CFG_HUMIDITY_0_FAN] = settings.ctrl.humidity_fan_level.item[0].level;
      in[CFG_HUMIDITY_1_VAL] = settings.ctrl.humidity_fan_level.item[1].val;
      in[CFG_HUMIDITY_1_FAN] = settings.ctrl.humidity_fan_level.item[1].level;
      in[CFG_HUMIDITY_2_VAL] = settings.ctrl.humidity_fan_level.item[2].val;
      in[CFG_HUMIDITY_2_FAN] = settings.ctrl.humidity_fan_level.item[2].level;
    }
    else
    {
      IMSG(LM_THING, "Dashboard writes ctrl..");
      settings.ctrl.temp_fan_level.enabled = in[CFG_TEMP_FAN_LEVEL_ENABLED];
      settings.ctrl.humidity_fan_level.enabled = in[CFG_HUMIDITY_FAN_LEVEL_ENABLED];
      settings.ctrl.humidity_fan_off.enabled = in[CFG_HUMIDITY_FAN_OFF_ENABLED];
      settings.ctrl.humidity_fan_off.abs_min_start = in[CFG_HUMIDITY_FAN_OFF_ABS_MIN_START];
      settings.ctrl.humidity_fan_off.abs_min_stop = in[CFG_HUMIDITY_FAN_OFF_ABS_MIN_STOP];
      settings.ctrl.humidity_fan_off.rel_min_start = in[CFG_HUMIDITY_FAN_OFF_REL_MIN_STOP];
      settings.ctrl.temp_fan_level.item[0].val = in[CFG_TEMP_0_VAL];
      settings.ctrl.temp_fan_level.item[0].level = in[CFG_TEMP_0_FAN];
      settings.ctrl.temp_fan_level.item[1].val = in[CFG_TEMP_1_VAL];
      settings.ctrl.temp_fan_level.item[1].level = in[CFG_TEMP_1_FAN];
      settings.ctrl.temp_fan_level.item[2].val = in[CFG_TEMP_2_VAL];
      settings.ctrl.temp_fan_level.item[2].level = in[CFG_TEMP_2_FAN];
      settings.ctrl.humidity_fan_level.item[0].val = in[CFG_HUMIDITY_0_VAL];
      settings.ctrl.humidity_fan_level.item[0].level = in[CFG_HUMIDITY_0_FAN];
      settings.ctrl.humidity_fan_level.item[1].val = in[CFG_HUMIDITY_1_VAL];
      settings.ctrl.humidity_fan_level.item[1].level = in[CFG_HUMIDITY_1_FAN];
      settings.ctrl.humidity_fan_level.item[2].val = in[CFG_HUMIDITY_2_VAL];
      settings.ctrl.humidity_fan_level.item[2].level = in[CFG_HUMIDITY_2_FAN];
      force_update = true;
    }
  };

  thing["state"] >> [](pson &out)
  {
    out["temp_fresh_in"] = state.temp.fresh_in;
    out["temp_fresh_out"] = state.temp.fresh_out;
    out["temp_exhaust_in"] = state.temp.exhaust_in;
    out["temp_exhaust_out"] = state.temp.exhaust_out;
    out["mode"] = (uint8_t)settings.mode;
    out["submode"] = (uint8_t)state.submode_auto;
    out["humidity_rel_exaust_in"] = state.humidity.rel_exaust_in;
    out["humidity_rel_fresh_out"] = state.humidity.rel_fresh_out;
    out["humidity_abs_exaust_in"] = state.humidity.abs_exaust_in;
    out["humidity_abs_fresh_out"] = state.humidity.abs_fresh_out;
    out["humidity_abs_delta"] = state.humidity.abs_delta;
    out["effectiveness"] = state.effectiveness;
    out["fan_level_fresh"] = state.actuator.level_fan_fresh;
    out["fan_level_exhaust"] = state.actuator.level_fan_exhaust;
    out["fan_level_frost"] = state.actuator.level_fan_frost;
    out["open_flap_frost"] = state.actuator.open_flap_frost;
    out["hours"] = now() / (1000.0 * 60 * 60);
    out["volume_fresh"] = state.volume_fresh;
    out["volume_exhaust"] = state.volume_exhaust;
    out["liter"] = state.liter;
    out["use_calibration"] = settings.use_calibration;
    out["ctrl_active_humidity_fan_off"] = state.ctrl_active.humidity_fan_off;
    out["ctrl_active_humidity_fan_level"] = state.ctrl_active.humidity_fan_level;
    out["ctrl_active_temp_fan_level"] = state.ctrl_active.temp_fan_level;
    out["ctrl_active_mode_auto"] = (uint8_t)state.submode_auto;
  };

  thing["state_raw"] >> [](pson &out)
  {
    out["temp_fresh_in"] = state_raw.temp.fresh_in;
    out["temp_fresh_out"] = state_raw.temp.fresh_out;
    out["temp_exhaust_in"] = state_raw.temp.exhaust_in;
    out["temp_exhaust_out"] = state_raw.temp.exhaust_out;
    out["humidity_rel_exaust_in"] = state_raw.humidity.rel_exaust_in;
    out["humidity_rel_fresh_out"] = state_raw.humidity.rel_fresh_out;
  };
}
