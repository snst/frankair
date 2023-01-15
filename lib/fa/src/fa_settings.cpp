// Copyright (c) 2023 by Stefan Schmidt
#include "fa_settings.h"
#include "fa_common.h"

void settingsSanitize()
{
    settings.ctrl.temp_fan_curve.enabled_points = toRange(settings.ctrl.temp_fan_curve.enabled_points, FAN_CURVE_POINTS_MIN, FAN_CURVE_POINTS_MAX);
    settings.ctrl.humidity_fan_curve.enabled_points = toRange(settings.ctrl.humidity_fan_curve.enabled_points, FAN_CURVE_POINTS_MIN, FAN_CURVE_POINTS_MAX);
    settings.ctrl.frost_fan_curve.enabled_points = toRange(settings.ctrl.frost_fan_curve.enabled_points, FAN_CURVE_POINTS_MIN, FAN_CURVE_POINTS_MAX);
}