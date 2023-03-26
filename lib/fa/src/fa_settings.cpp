// Copyright (c) 2023 by Stefan Schmidt
#include "fa_settings.h"
#include "fa_common.h"
#include <cstring>

fa_settings_t settings;

void settingsSanitize()
{
    settings.ctrl.temp_fan_curve.enabled_points = toRange(settings.ctrl.temp_fan_curve.enabled_points, FAN_CURVE_POINTS_MIN, FAN_CURVE_POINTS_MAX);
    settings.ctrl.humidity_fan_curve.enabled_points = toRange(settings.ctrl.humidity_fan_curve.enabled_points, FAN_CURVE_POINTS_MIN, FAN_CURVE_POINTS_MAX);
    settings.ctrl.frost_fan_curve.enabled_points = toRange(settings.ctrl.frost_fan_curve.enabled_points, FAN_CURVE_POINTS_MIN, FAN_CURVE_POINTS_MAX);
    settings.ctrl.frost_flap_ctrl.level_open = toRange(settings.ctrl.frost_flap_ctrl.level_open, FLAP_LEVEL_CLOSE, FLAP_LEVEL_OPEN);
    settings.stream_interval_min = toRange(settings.stream_interval_min, 1U, 255U);
}

void settingsDefault()
{
    IMSG(LINFO, "settingsDefault()");
    memset(&settings, 0U, sizeof(settings));
    settings.mode = (uint8_t)controller_mode_t::kAuto;
    settings.controller_interval_sec = 10U;      // sec
    settings.temp_sensor_read_interval_sec = 5U; // sec
    settings.stream_interval_min = 10U;
    settings.measurement_alpha_sniff = 0.8f;
    settings.measurement_alpha_on = 0.5f;
    settings.log_mask = LERROR | LINFO | LIOT | LSM | LACTUATOR;
    
    settings.use_calibration_humidity = false;
    settings.use_calibrated_temp_for_abs_humidity = false;
    settings.use_calibration_temp = false;
    
    settings.ctrl.fan_offset_exhaust = 0;
    settings.ctrl.fan_offset_fresh = 0;
    settings.ctrl.fan_frost_level_max = FAN_LEVEL_MAX;
    settings.ctrl.fan_frost_level_min = FAN_LEVEL_OFF;
    settings.ctrl.fan_level_max = FAN_LEVEL_MAX;
    settings.ctrl.fan_level_min = FAN_LEVEL_OFF;

    settings.ctrl.frost_fan_curve.enabled = true;
    settings.ctrl.frost_fan_curve.enabled_points = 3U;
    settings.ctrl.frost_fan_curve.item[0].val = 0.0f;
    settings.ctrl.frost_fan_curve.item[0].level = 0U;
    settings.ctrl.frost_fan_curve.item[1].val = -2.01f;
    settings.ctrl.frost_fan_curve.item[1].level = 4U;
    settings.ctrl.frost_fan_curve.item[2].val = -6.01f;
    settings.ctrl.frost_fan_curve.item[2].level = 8U;

    settings.ctrl.frost_flap_ctrl.enabled = true;
    settings.ctrl.frost_flap_ctrl.temp_min_close = 2.0f;
    settings.ctrl.frost_flap_ctrl.temp_min_open = 0.0f;
    settings.ctrl.frost_flap_ctrl.level_open = FLAP_LEVEL_OPEN;

    settings.ctrl.humidity_fan_curve.enabled = true;
    settings.ctrl.humidity_fan_curve.enabled_points = 4U;
    settings.ctrl.humidity_fan_curve.item[0].val = 4.0f;
    settings.ctrl.humidity_fan_curve.item[0].level = 10U;
    settings.ctrl.humidity_fan_curve.item[1].val = 3.0f;
    settings.ctrl.humidity_fan_curve.item[1].level = 8U;
    settings.ctrl.humidity_fan_curve.item[2].val = 2.0f;
    settings.ctrl.humidity_fan_curve.item[2].level = 5U;
    settings.ctrl.humidity_fan_curve.item[3].val = 0.5f;
    settings.ctrl.humidity_fan_curve.item[3].level = 0U;

    settings.ctrl.temp_fan_curve.enabled = true;
    settings.ctrl.temp_fan_curve.enabled_points = 4U;
    settings.ctrl.temp_fan_curve.item[0].val = 17.0f;
    settings.ctrl.temp_fan_curve.item[0].level = 10U;
    settings.ctrl.temp_fan_curve.item[1].val = 15.0f;
    settings.ctrl.temp_fan_curve.item[1].level = 8U;
    settings.ctrl.temp_fan_curve.item[2].val = 10.0f;
    settings.ctrl.temp_fan_curve.item[2].level = 5U;
    settings.ctrl.temp_fan_curve.item[3].val = 5.0f;
    settings.ctrl.temp_fan_curve.item[3].level = 0U;

    settings.ctrl.simple.desired_temp_min = 10U;
    settings.ctrl.simple.desired_temp_max = 22U;
    settings.ctrl.simple.abs_hum_min_on = 1.0f;   // g/m³
    settings.ctrl.simple.abs_hum_min_wait = 0.5f; // g/m³
    settings.ctrl.simple.rel_min_wait = 50.0f;    // rel %
    settings.ctrl.simple.rel_min_enabled = true;
    settings.ctrl.simple.abs_min_enabled = true;
    settings.ctrl.simple.desired_temp_min_enabled = true;
    settings.ctrl.simple.desired_temp_max_enabled = true;

    settings.sniff.enabled = true;
    settings.sniff.fan_level_sniff = 5U;
    settings.sniff.fan_level_wait = 0U;
    settings.sniff.wait_sec = 30U * 60U;
    settings.sniff.sniff_sec = 60U;

    settingsDefaultPlatform();
}
