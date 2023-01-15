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
#include "fa_version.h"

ThingerESP32 thing(FA_USERNAME, FA_DEVICE_ID, FA_DEVICE_CREDENTIAL);

void thingUpdate()
{
  thing.handle();
  state.is_online = thing.is_connected();
}

template <typename T>
void valUpdate(protoson::pson &in, const char *name, T &val, bool isEmpty)
{
  auto &remote = in[name];
  if (isEmpty) // remote.is_empty())
  {
    remote = val;
  }
  else
  {
    val = remote;
  }
}

void thingSetup()
{
  thing.add_wifi(FA_SSID, FA_SSID_PASSWORD);

  ADD_CMD("_Reboot", ESP.restart)
  ADD_CMD("_Settings save", settingsWrite)
  ADD_CMD("_Settings load", settingsLoad)
  ADD_CMD("_Settings clear", settingsClear)
  ADD_CMD("_Calibration save", calibrationWrite)
  ADD_CMD("_Calibration load", calibrationLoad)
  ADD_CMD("_Calibration clear", calibrationClear)
  ADD_CMD("_Scan sensors", sensorsScan)
  ADD_CMD("_Calibrate temp min", sensorsCalibrateTempLow)
  ADD_CMD("_Calibrate temp max", sensorsCalibrateTempHigh)
  ADD_CMD("_Calibrate humidity min", sensorsCalibrateHumidityLow)
  ADD_CMD("_Calibrate humidity max", sensorsCalibrateHumidityHigh)

  thing["Settings"] << [](pson &in)
  {
    bool isEmpty = in.is_empty();

    valUpdate(in, "1.1 Controller mode", settings.mode, isEmpty);
    valUpdate(in, "1.2 Controller intervall sec", settings.controller_interval_sec, isEmpty);
    valUpdate(in, "1.3 Sensor read intervall sec", settings.temp_sensor_read_interval_sec, isEmpty);
    valUpdate(in, "1.4 Sensor alpha filter", settings.measurement_alpha, isEmpty);
    valUpdate(in, "1.5 Enable sensor calibration temp", settings.use_calibration_temp, isEmpty);
    valUpdate(in, "1.6 Enable sensor calibration humidity", settings.use_calibration_humidity, isEmpty);
    valUpdate(in, "1.7 Log mask", settings.log_mask, isEmpty);

    valUpdate(in, "2.1 Sniffing enabled", settings.sniff.enabled, isEmpty);
    valUpdate(in, "2.2 Sniffing fan level", settings.sniff.fan_level, isEmpty);
    valUpdate(in, "2.3 Sniffing duration sec", settings.sniff.duration_sec, isEmpty);
    valUpdate(in, "2.4 Sniffing intervall sec", settings.sniff.interval_sec, isEmpty);

    valUpdate(in, "3.1 Manual fan level fresh", settings.manual.level_fan_fresh, isEmpty);
    valUpdate(in, "3.2 Manual fan level exhaust", settings.manual.level_fan_exhaust, isEmpty);
    valUpdate(in, "3.3 Manual fan level frost", settings.manual.level_fan_frost, isEmpty);
    valUpdate(in, "3.4 Manual frost flap open", settings.manual.open_flap_frost, isEmpty);

    valUpdate(in, "4.1 Fan level main min", settings.ctrl.fan_level_min, isEmpty);
    valUpdate(in, "4.2 Fan level main max", settings.ctrl.fan_level_max, isEmpty);
    valUpdate(in, "4.3 Fan level frost min", settings.ctrl.fan_frost_level_min, isEmpty);
    valUpdate(in, "4.4 Fan level frost max", settings.ctrl.fan_frost_level_max, isEmpty);

    force_update = !isEmpty;
  };

  thing["Calibration sensor"] << [](pson &in)
  {
    bool isEmpty = in.is_empty();
    valUpdate(in, "1.1 Temp exhaust in min", fa_calibration_sensor.exhaust_in_temp.min, isEmpty);
    valUpdate(in, "1.2 Temp exhaust in max", fa_calibration_sensor.exhaust_in_temp.max, isEmpty);
    valUpdate(in, "1.3 Temp exhaust out min", fa_calibration_sensor.exhaust_out_temp.min, isEmpty);
    valUpdate(in, "1.4 Temp exhaust out max", fa_calibration_sensor.exhaust_out_temp.max, isEmpty);
    valUpdate(in, "2.1 Temp fresh in min", fa_calibration_sensor.fresh_in_temp.min, isEmpty);
    valUpdate(in, "2.2 Temp fresh in max", fa_calibration_sensor.fresh_in_temp.max, isEmpty);
    valUpdate(in, "2.3 Temp fresh out min", fa_calibration_sensor.fresh_out_temp.min, isEmpty);
    valUpdate(in, "2.4 Temp fresh out max", fa_calibration_sensor.fresh_out_temp.max, isEmpty);
    valUpdate(in, "3.1 Humidity exhaust in min", fa_calibration_sensor.exhaust_in_humidity.min, isEmpty);
    valUpdate(in, "3.2 Humidity exhaust in max", fa_calibration_sensor.exhaust_in_humidity.max, isEmpty);
    valUpdate(in, "3.3 Humidity fresh out min", fa_calibration_sensor.fresh_out_humidity.min, isEmpty);
    valUpdate(in, "3.4 Humidity fresh out max", fa_calibration_sensor.fresh_out_humidity.max, isEmpty);

    force_update = !isEmpty;
  };

  thing["Calibration actuator"] << [](pson &in)
  {
    bool isEmpty = in.is_empty();
    valUpdate(in, "1.1 Frost flap pos min", fa_calibration_actuator.flap_pos.min, isEmpty);
    valUpdate(in, "1.2 Frost flap pos max", fa_calibration_actuator.flap_pos.max, isEmpty);

    protoson::pson_array &array_fan_main = in["1.3 Fan main level-pwm"];
    protoson::pson_array &array_fan_frost = in["1.4 Fan frost level-pwm"];

    for (uint8_t i = 0U; i < FAN_LEVEL_STEPS; i++)
    {
      if (isEmpty)
      {
        array_fan_main.add(fa_calibration_actuator.fan_pwm_main[i]);
        array_fan_frost.add(fa_calibration_actuator.fan_pwm_frost[i]);
      }
      else
      {
        fa_calibration_actuator.fan_pwm_main[i] = *array_fan_main[i];
        fa_calibration_actuator.fan_pwm_frost[i] = *array_fan_frost[i];
      }
    }
    force_update = !isEmpty;
  };

  thing["ctrl"] << [](pson &in)
  {
    bool isEmpty = in.is_empty();
    valUpdate(in, "1.1 Enable general humidity conditions for fan", settings.ctrl.humidity_fan_ctrl.enabled, isEmpty);
    valUpdate(in, "1.2 Start fan if abs humidity delta greater than g/m³", settings.ctrl.humidity_fan_ctrl.abs_min_start, isEmpty);
    valUpdate(in, "1.3 Stop fan if abs humidity delta less than g/m³", settings.ctrl.humidity_fan_ctrl.abs_min_stop, isEmpty);
    valUpdate(in, "1.4 Stop fan if rel humidity less than %", settings.ctrl.humidity_fan_ctrl.rel_min_start, isEmpty);
    valUpdate(in, "2.1 Enabled temp-fan curve", settings.ctrl.temp_fan_curve.enabled, isEmpty);
    valUpdate(in, "2.2 Used temp-fan curve points", settings.ctrl.temp_fan_curve.enabled_points, isEmpty);
    valUpdate(in, "3.1 Enabled humidity-fan curve", settings.ctrl.humidity_fan_curve.enabled, isEmpty);
    valUpdate(in, "3.2 Used humidity-fan curve points", settings.ctrl.humidity_fan_curve.enabled_points, isEmpty);
    valUpdate(in, "4.1 Enable frost flap", settings.ctrl.frost_flap_ctrl.enabled, isEmpty);
    valUpdate(in, "4.2 Open flap if temp is lesser than °C", settings.ctrl.frost_flap_ctrl.temp_min_open, isEmpty);
    valUpdate(in, "4.3 Close flap if temp is greater than °C", settings.ctrl.frost_flap_ctrl.temp_min_close, isEmpty);
    valUpdate(in, "5.1 Enabled frost-fan curve", settings.ctrl.frost_fan_curve.enabled, isEmpty);
    valUpdate(in, "5.2 Used frost-fan curve points", settings.ctrl.frost_fan_curve.enabled_points, isEmpty);
    settingsSanitize();

    protoson::pson_array &array_temp_temp = in["2.3 Temp less than °C"];
    protoson::pson_array &array_temp_level = in["2.4 Fan level"];
    protoson::pson_array &array_hum_hum = in["3.3 Abs humidity delta less than g/m³"];
    protoson::pson_array &array_hum_level = in["3.4 Fan level"];
    protoson::pson_array &array_frost_temp_temp = in["5.3 Temp less than °C"];
    protoson::pson_array &array_frost_temp_level = in["5.4 Fan level"];

    for (uint8_t i = 0U; i < settings.ctrl.temp_fan_curve.enabled_points; i++)
    {
      if (isEmpty)
      {
        array_temp_temp.add(settings.ctrl.temp_fan_curve.item[i].val);
        array_temp_level.add(settings.ctrl.temp_fan_curve.item[i].level);
      }
      else
      {
        settings.ctrl.temp_fan_curve.item[i].val = *array_temp_temp[i];
        settings.ctrl.temp_fan_curve.item[i].level = *array_temp_level[i];
      }
    }
    for (uint8_t i = 0U; i < settings.ctrl.humidity_fan_curve.enabled_points; i++)
    {
      if (isEmpty)
      {
        array_hum_hum.add(settings.ctrl.humidity_fan_curve.item[i].val);
        array_hum_level.add(settings.ctrl.humidity_fan_curve.item[i].level);
      }
      else
      {
        settings.ctrl.humidity_fan_curve.item[i].val = *array_hum_hum[i];
        settings.ctrl.humidity_fan_curve.item[i].level = *array_hum_level[i];
      }
    }
    for (uint8_t i = 0U; i < settings.ctrl.frost_fan_curve.enabled_points; i++)
    {
      if (isEmpty)
      {
        array_frost_temp_temp.add(settings.ctrl.frost_fan_curve.item[i].val);
        array_frost_temp_level.add(settings.ctrl.frost_fan_curve.item[i].level);
      }
      else
      {
        settings.ctrl.frost_fan_curve.item[i].val = *array_frost_temp_temp[i];
        settings.ctrl.frost_fan_curve.item[i].level = *array_frost_temp_temp[i];
      }
    }

    force_update = !isEmpty;
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
    out["ctrl_active_humidity_fan_off"] = state.ctrl_active.humidity_fan_off;
    out["ctrl_active_humidity_fan_curve"] = state.ctrl_active.humidity_fan_curve;
    out["ctrl_active_temp_fan_curve"] = state.ctrl_active.temp_fan_curve;
    out["ctrl_active_frost_fan_curve"] = state.ctrl_active.frost_fan_curve;
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

  thing["Info"] >> [](pson &out)
  {
    out["version"] = FA_VERSION;
    out["build"] = __DATE__ "  " __TIME__;
  };
}
