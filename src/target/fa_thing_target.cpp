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
#include "fa_ota.h"
#include "fa_statistic.h"
#include "fa_error.h"
#include "fa_sm_commands.h"

FAThingerESP32 thing(FA_USERNAME, FA_DEVICE_ID, FA_DEVICE_CREDENTIAL);

void thingUpdate()
{
	if (state.is_online != thing.is_connected())
	{
		state.is_online = thing.is_connected();
		if (state.is_online)
		{
			thingCallEndpoint("welcome");
		}
	}

	if (!state.is_online)
	{
		if (thing.connect_network())
		{
			thing.handle();
		}
	}
	else
	{
		thing.handle();
	}
}

template <typename T>
void valUpdate(protoson::pson &in, const char *name, T &val, bool isEmpty)
{
	auto &remote = in[name];
	if (isEmpty)
	{
		remote = val;
	}
	else
	{
		cmdSettingsChanged();
		val = remote;
	}
}

void thingSetupPlatform()
{
	thing.add_wifi2(FA_SSID_1, FA_SSID_PASSWORD_1);
	thing.add_wifi2(FA_SSID_2, FA_SSID_PASSWORD_2);

	thing["Logging"] << [](pson &in)
	{
		bool isEmpty = in.is_empty();
		setMask(settings.log_mask);
		valUpdate(in, "1 Error", log_mask.error, isEmpty);
		valUpdate(in, "2 Info", log_mask.info, isEmpty);
		valUpdate(in, "3 Debug", log_mask.debug, isEmpty);
		valUpdate(in, "4 Sensor", log_mask.sensor, isEmpty);
		valUpdate(in, "5 Actuator", log_mask.actuator, isEmpty);
		valUpdate(in, "6 IOT", log_mask.iot, isEmpty);
		valUpdate(in, "7 SM", log_mask.sm, isEmpty);
		settings.log_mask = getMask();
	};

	thing["Dobson override"] << [](pson &in)
	{
		bool isEmpty = in.is_empty();
		valUpdate(in, "1 Enabled", override.enabled, isEmpty);
		if (!override.enabled)
		{
			override.temp.exhaust_in = round2(state.temp.exhaust_in);
			override.temp.fresh_in = round2(state.temp.fresh_in);
			override.temp.exhaust_out = round2(state.temp.exhaust_out);
			override.temp.fresh_out = round2(state.temp.fresh_out);
			override.humidity_rel_exhaust_in = round2(state.humidity.rel_exhaust_in);
			override.humidity_rel_fresh_out = round2(state.humidity.rel_fresh_out);
		}
		valUpdate(in, "2 Temp exhaust IN", override.temp.exhaust_in, isEmpty);
		valUpdate(in, "3 Temp fresh IN", override.temp.fresh_in, isEmpty);
		valUpdate(in, "4 Temp exhaust OUT", override.temp.exhaust_out, isEmpty);
		valUpdate(in, "5 Temp fresh OUT", override.temp.fresh_out, isEmpty);
		valUpdate(in, "6 Humidity exhaust IN", override.humidity_rel_exhaust_in, isEmpty);
		valUpdate(in, "7 Humidity fresh OUT", override.humidity_rel_fresh_out, isEmpty);
	};

	thing["Manual control"] << [](pson &in)
	{
		bool isEmpty = in.is_empty();
		valUpdate(in, "1 Fan level fresh", settings.manual.level_fan_fresh, isEmpty);
		valUpdate(in, "2 Fan level exhaust", settings.manual.level_fan_exhaust, isEmpty);
		valUpdate(in, "3 Fan level frost", settings.manual.level_fan_frost, isEmpty);
		valUpdate(in, "4 Frost flap open", settings.manual.open_flap_frost, isEmpty);
		settingsSanitize();
	};

	thing["Settings"] << [](pson &in)
	{
		bool isEmpty = in.is_empty();
		valUpdate(in, "1.1 Controller mode", settings.mode, isEmpty);
		valUpdate(in, "1.2 Controller interval sec", settings.controller_interval_sec, isEmpty);
		valUpdate(in, "1.3 Sensor read interval sec", settings.sensor_read_interval_sec, isEmpty);
		valUpdate(in, "1.4 Bucket stream interval min", settings.stream_interval_min, isEmpty);
		valUpdate(in, "2.1 Sensor alpha filter sniff", settings.measurement_alpha_sniff, isEmpty);
		valUpdate(in, "2.2 Sensor alpha filter on", settings.measurement_alpha_on, isEmpty);
		valUpdate(in, "3.1 Enable sensor calibration temp", settings.use_calibration_temp, isEmpty);
		valUpdate(in, "3.2 Enable sensor calibration humidity", settings.use_calibration_humidity, isEmpty);
		valUpdate(in, "3.3 Use calibrated temp for abs humidity", settings.use_calibrated_temp_for_abs_humidity, isEmpty);
		valUpdate(in, "4.1 Fan offset exhaust", settings.ctrl.fan_offset_exhaust, isEmpty);
		valUpdate(in, "4.2 Fan offset fresh", settings.ctrl.fan_offset_fresh, isEmpty);
		valUpdate(in, "5.1 Enable sniffing", settings.sniff.enabled, isEmpty);
		valUpdate(in, "5.2 Fan level sniff", settings.sniff.fan_level_sniff, isEmpty);
		valUpdate(in, "5.3 Fan level wait", settings.sniff.fan_level_wait, isEmpty);
		valUpdate(in, "5.4 Seconds sniff", settings.sniff.sniff_sec, isEmpty);
		valUpdate(in, "5.5 Seconds wait", settings.sniff.wait_sec, isEmpty);
		
		settingsSanitize();
	};


	thing["Settings simple control"] << [](pson &in)
	{
		bool isEmpty = in.is_empty();
		valUpdate(in, "1.1 Enable abs humidity conditions", settings.ctrl.simple.abs_min_enabled, isEmpty);
		valUpdate(in, "1.2 Start fan if abs humidity delta greater than g/m³", settings.ctrl.simple.abs_hum_min_on, isEmpty);
		valUpdate(in, "1.3 Stop fan if abs humidity delta less than g/m³", settings.ctrl.simple.abs_hum_min_wait, isEmpty);
		valUpdate(in, "2.1 Enable rel humidity conditions", settings.ctrl.simple.rel_min_enabled, isEmpty);
		valUpdate(in, "2.2 Stop fan if rel humidity less than %", settings.ctrl.simple.rel_min_wait, isEmpty);
		valUpdate(in, "3.1 Stop fan if room temp < desired min temp and fresh temp below room temp", settings.ctrl.simple.desired_temp_min_enabled, isEmpty);
		valUpdate(in, "3.2 Desired min temp", settings.ctrl.simple.desired_temp_min, isEmpty);
		valUpdate(in, "4.1 Fan max level if room temp < desired max temp and fresh temp > room temp", settings.ctrl.simple.desired_temp_max_enabled, isEmpty);
		valUpdate(in, "4.2 Desired max temp", settings.ctrl.simple.desired_temp_max, isEmpty);
		settingsSanitize();
	};

	thing["Settings temperature curve"] << [](pson &in)
	{
		bool isEmpty = in.is_empty();
		valUpdate(in, "1 Enabled temp-fan curve", settings.ctrl.temp_fan_curve.enabled, isEmpty);
		valUpdate(in, "2 Used temp-fan curve points", settings.ctrl.temp_fan_curve.enabled_points, isEmpty);
		protoson::pson_array &array_temp_temp = in["3 Temp °C"];
		protoson::pson_array &array_temp_level = in["4 Fan level"];
		for (uint8_t i = 0U; i < settings.ctrl.temp_fan_curve.enabled_points; i++)
		{
			if (isEmpty)
			{
				array_temp_temp.add(settings.ctrl.temp_fan_curve.item[i].val);
				array_temp_level.add(settings.ctrl.temp_fan_curve.item[i].level);
			}
			else
			{
				if (array_temp_temp[i])
				{
					settings.ctrl.temp_fan_curve.item[i].val = *array_temp_temp[i];
				}
				if (array_temp_level[i])
				{
					settings.ctrl.temp_fan_curve.item[i].level = *array_temp_level[i];
				}
			}
		}
		settingsSanitize();
	};

	thing["Settings humidity curve"] << [](pson &in)
	{
		bool isEmpty = in.is_empty();
		valUpdate(in, "1 Enabled humidity-fan curve", settings.ctrl.humidity_fan_curve.enabled, isEmpty);
		valUpdate(in, "2 Number of humidity-fan curve points", settings.ctrl.humidity_fan_curve.enabled_points, isEmpty);
		protoson::pson_array &array_hum_hum = in["3 Abs humidity delta g/m³"];
		protoson::pson_array &array_hum_level = in["4 Fan level"];
		for (uint8_t i = 0U; i < settings.ctrl.humidity_fan_curve.enabled_points; i++)
		{
			if (isEmpty)
			{
				array_hum_hum.add(settings.ctrl.humidity_fan_curve.item[i].val);
				array_hum_level.add(settings.ctrl.humidity_fan_curve.item[i].level);
			}
			else
			{
				if (array_hum_hum[i])
				{
					settings.ctrl.humidity_fan_curve.item[i].val = *array_hum_hum[i];
				}
				if (array_hum_level[i])
				{
					settings.ctrl.humidity_fan_curve.item[i].level = *array_hum_level[i];
				}
			}
		}
		settingsSanitize();
	};

	thing["Settings frost"] << [](pson &in)
	{
		bool isEmpty = in.is_empty();
		valUpdate(in, "1 Enable frost flap", settings.ctrl.frost_flap_ctrl.enabled, isEmpty);
		valUpdate(in, "2 Open flap if fresh in temp is lesser than °C", settings.ctrl.frost_flap_ctrl.temp_min_open, isEmpty);
		valUpdate(in, "3 Open flap to pos", settings.ctrl.frost_flap_ctrl.level_open, isEmpty);
		valUpdate(in, "4 Close flap if fresh in temp is greater than °C", settings.ctrl.frost_flap_ctrl.temp_min_close, isEmpty);
		valUpdate(in, "5 Enabled frost-fan curve", settings.ctrl.frost_fan_curve.enabled, isEmpty);
		valUpdate(in, "6 Number of frost-fan curve points", settings.ctrl.frost_fan_curve.enabled_points, isEmpty);
		protoson::pson_array &array_frost_temp_temp = in["7 Fresh IN temp °C"];
		protoson::pson_array &array_frost_temp_level = in["8 Fan level"];
		for (uint8_t i = 0U; i < settings.ctrl.frost_fan_curve.enabled_points; i++)
		{
			if (isEmpty)
			{
				array_frost_temp_temp.add(settings.ctrl.frost_fan_curve.item[i].val);
				array_frost_temp_level.add(settings.ctrl.frost_fan_curve.item[i].level);
			}
			else
			{
				if (array_frost_temp_temp[i])
				{
					settings.ctrl.frost_fan_curve.item[i].val = *array_frost_temp_temp[i];
				}
				if (array_frost_temp_level[i])
				{
					settings.ctrl.frost_fan_curve.item[i].level = *array_frost_temp_level[i];
				}
			}
		}
		settingsSanitize();
	};

	thing["Calibration temperature"] << [](pson &in)
	{
		bool isEmpty = in.is_empty();
		valUpdate(in, "1.1 Temp reference min", calibration_sensor.ref_temp.min, isEmpty);
		valUpdate(in, "1.2 Temp exhaust in min", calibration_sensor.exhaust_in_temp.min, isEmpty);
		valUpdate(in, "1.3 Temp exhaust out min", calibration_sensor.exhaust_out_temp.min, isEmpty);
		valUpdate(in, "1.4 Temp fresh in min", calibration_sensor.fresh_in_temp.min, isEmpty);
		valUpdate(in, "1.5 Temp fresh out min", calibration_sensor.fresh_out_temp.min, isEmpty);
		valUpdate(in, "2.1 Temp reference max", calibration_sensor.ref_temp.max, isEmpty);
		valUpdate(in, "2.2 Temp exhaust in max", calibration_sensor.exhaust_in_temp.max, isEmpty);
		valUpdate(in, "2.3 Temp exhaust out max", calibration_sensor.exhaust_out_temp.max, isEmpty);
		valUpdate(in, "2.4 Temp fresh in max", calibration_sensor.fresh_in_temp.max, isEmpty);
		valUpdate(in, "2.5 Temp fresh out max", calibration_sensor.fresh_out_temp.max, isEmpty);
		settingsSanitize();
	};

	thing["Calibration humidity"] << [](pson &in)
	{
		bool isEmpty = in.is_empty();
		valUpdate(in, "1.1 Humidity reference min", calibration_sensor.ref_humidity.min, isEmpty);
		valUpdate(in, "1.2 Humidity exhaust in min", calibration_sensor.exhaust_in_humidity.min, isEmpty);
		valUpdate(in, "1.3 Humidity fresh out min", calibration_sensor.fresh_out_humidity.min, isEmpty);
		valUpdate(in, "2.1 Humidity reference max", calibration_sensor.ref_humidity.max, isEmpty);
		valUpdate(in, "2.2 Humidity exhaust in max", calibration_sensor.exhaust_in_humidity.max, isEmpty);
		valUpdate(in, "2.3 Humidity fresh out max", calibration_sensor.fresh_out_humidity.max, isEmpty);
		settingsSanitize();
	};

	thing["Settings actuator limits"] << [](pson &in)
	{
		bool isEmpty = in.is_empty();
		valUpdate(in, "1.1 Fan level main min", settings.ctrl.fan_level_min, isEmpty);
		valUpdate(in, "1.2 Fan level main max", settings.ctrl.fan_level_max, isEmpty);
		valUpdate(in, "2.1 Fan level frost min", settings.ctrl.fan_frost_level_min, isEmpty);
		valUpdate(in, "2.2 Fan level frost max", settings.ctrl.fan_frost_level_max, isEmpty);
		valUpdate(in, "3.1 Frost flap min", calibration_actuator.flap_pos.min, isEmpty);
		valUpdate(in, "3.2 Frost flap max", calibration_actuator.flap_pos.max, isEmpty);
		settingsSanitize();
	};

	thing["Calibration fan main pwm"] << [](pson &in)
	{
		bool isEmpty = in.is_empty();
		valUpdate(in, "1 Frequency", calibration_actuator.fan_freq_main, isEmpty);
		protoson::pson_array &array_fan_main = in["2 Level-pwm"];
		for (uint8_t i = 0U; i < FAN_LEVEL_STEPS; i++)
		{
			if (isEmpty)
			{
				array_fan_main.add(calibration_actuator.fan_pwm_main[i]);
			}
			else
			{
				if (array_fan_main[i])
				{
					calibration_actuator.fan_pwm_main[i] = *array_fan_main[i];
				}
			}
		}
		settingsSanitize();
	};

	thing["Calibration fan frost pwm"] << [](pson &in)
	{
		bool isEmpty = in.is_empty();
		valUpdate(in, "1 Frequency", calibration_actuator.fan_freq_frost, isEmpty);
		protoson::pson_array &array_fan_frost = in["2 Level-pwm"];
		for (uint8_t i = 0U; i < FAN_LEVEL_STEPS; i++)
		{
			if (isEmpty)
			{
				array_fan_frost.add(calibration_actuator.fan_pwm_frost[i]);
			}
			else
			{
				if (array_fan_frost[i])
				{
					calibration_actuator.fan_pwm_frost[i] = *array_fan_frost[i];
				}
			}
		}
		settingsSanitize();
	};

	thing["Calibration fan main volume"] << [](pson &in)
	{
		bool isEmpty = in.is_empty();
		valUpdate(in, "1 Test volume liter", calibration_actuator.calibration_volume_liter, isEmpty);
		protoson::pson_array &array_fan_main_sec = in["2 Needed sec"];
		for (uint8_t i = 0U; i < FAN_LEVEL_STEPS; i++)
		{
			if (isEmpty)
			{
				array_fan_main_sec.add(calibration_actuator.fan_cal_time_main[i]);
			}
			else
			{
				if (array_fan_main_sec[i])
				{
					calibration_actuator.fan_cal_time_main[i] = *array_fan_main_sec[i];
				}
			}
		}
		settingsSanitize();
	};

	thing["State"] >> [](pson &out)
	{
		out["temp_fresh_in"] = state.temp.fresh_in;
		out["temp_fresh_out"] = state.temp.fresh_out;
		out["temp_exhaust_in"] = state.temp.exhaust_in;
		out["temp_exhaust_out"] = state.temp.exhaust_out;
		out["mode"] = (uint8_t)settings.mode;
		out["sm_state"] = (uint8_t)state.sm_state;
		out["humidity_rel_exhaust_in"] = state.humidity.rel_exhaust_in;
		out["humidity_rel_fresh_out"] = state.humidity.rel_fresh_out;
		out["humidity_abs_exhaust_in"] = state.humidity.abs_exhaust_in;
		out["humidity_abs_fresh_out"] = state.humidity.abs_fresh_out;
		out["humidity_abs_delta"] = state.humidity.abs_delta;
		out["fan_level_fresh"] = state.actuator.level_fan_fresh;
		out["fan_level_exhaust"] = state.actuator.level_fan_exhaust;
		out["fan_level_frost"] = state.actuator.level_fan_frost;
		out["open_flap_frost"] = state.actuator.open_flap_frost;
		out["running_sec"] = state.running.sec;
		out["efficiency"] = state.efficiency;
		out["ctrl_active_decisions"] = state.ctrl_active.decisions;
		out["ctrl_active_humidity_fan_curve"] = state.ctrl_active.humidity_fan_curve;
		out["ctrl_active_temp_fan_curve"] = state.ctrl_active.temp_fan_curve;
		out["ctrl_active_frost_fan_curve"] = state.ctrl_active.frost_fan_curve;
		out["liter_per_hour"] = statistic.liter_per_hour;
		out["volume_m3_per_hour"] = statistic.volume_m3_per_hour;
		out["errors"] = state.errors;
	};

	thing["Statistics"] >> [](pson &out)
	{
		out["liter_sum"] = statistic.liter_sum;
		out["volume_m3_sum"] = statistic.volume_m3_sum;
		out["running_sec"] = statistic.running.sec;
	};

	thing["StateRAW"] >>
		[](pson &out)
	{
		out["temp_fresh_in"] = state_raw.temp.fresh_in;
		out["temp_fresh_out"] = state_raw.temp.fresh_out;
		out["temp_exhaust_in"] = state_raw.temp.exhaust_in;
		out["temp_exhaust_out"] = state_raw.temp.exhaust_out;
		out["humidity_rel_exhaust_in"] = state_raw.humidity.rel_exhaust_in;
		out["humidity_rel_fresh_out"] = state_raw.humidity.rel_fresh_out;
	};

	thing["Info"] >> [](pson &out)
	{
		out["version"] = FA_VERSION;
		out["build"] = __DATE__ "  " __TIME__;
	};

	thing["OTA"] >> [](pson &out)
	{
		out["firmware_size"] = ota.firmware_size;
		out["transferred_size"] = ota.transferred_size;
		out["checksum"] = ota.checksum;
		out["downloading"] = ota.downloading;
		out["abort"] = ota.abort;
		out["error"] = ota.error;
		out["success"] = ota.success;
		out["http_response"] = ota.http_response;
		out["simulate"] = ota.simulate;
	};

	ADD_CMD("a Start sniffing", cmdStartSniffing);
	ADD_CMD("b Stop sniffing", cmdStopSniffing);
	ADD_CMD("c Reboot", cmdReboot);
	ADD_CMD("d Reset statistic", statisticReset);
	ADD_CMD("e Clear errors", errorClear);
	ADD_CMD("f Settings save", settingsWrite);
	ADD_CMD("g Calibration save", calibrationWrite);
	ADD_CMD("h Start OTA update", otaStart);
	ADD_CMD("i Start OTA simulation", otaStartSim);
	ADD_CMD("j Abort OTA", otaAbort);
	ADD_CMD("k Calibrate temp min", sensorsCalibrateTempLow);
	ADD_CMD("l Calibrate temp max", sensorsCalibrateTempHigh);
	ADD_CMD("m Calibrate humidity min", sensorsCalibrateHumidityLow);
	ADD_CMD("n Calibrate humidity max", sensorsCalibrateHumidityHigh);
	ADD_CMD("o Settings load", settingsLoad);
	ADD_CMD("p Calibration load", calibrationLoad);
	ADD_CMD("q Settings clear", settingsClear);
	ADD_CMD("r Calibration clear", calibrationClear);
	ADD_CMD("s Settings default", settingsDefault);
	ADD_CMD("t Calibration default", calibrationDefault);
}

void thingCallEndpoint(const char *endpoint_name)
{
	thing.call_endpoint(endpoint_name, "");
}

void thingSendStreamState()
{
	thing.stream("State");
}
