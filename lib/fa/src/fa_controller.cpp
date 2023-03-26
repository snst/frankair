// Copyright (c) 2023 by Stefan Schmidt
#include "fa_common.h"
#include "fa_settings.h"
#include "fa_controller.h"
#include "fa_fan.h"
#include "fa_flap.h"
#include "fa_log.h"
#include "fa_statistic.h"
#include "fa_error.h"
#include "fa_thing.h"
#include "fa_sm.h"
#include <math.h>

fa_state_t state;
fa_state_raw_t state_raw;

bool reduceFanLevel(uint8_t &dest, uint8_t val)
{
  if (val < dest)
  {
    dest = val;
    return true;
  }
  return false;
}

bool increaseFanLevel(uint8_t &dest, uint8_t val)
{
  if (val > dest)
  {
    dest = val;
    return true;
  }
  return false;
}

bool updateFanLevel(uint8_t &dest, uint8_t val, bool reduce)
{
  if (reduce)
  {
    return reduceFanLevel(dest, val);
  }
  else
  {
    return increaseFanLevel(dest, val);
  }
}

bool calcFanLevelCurve(float input, fa_fan_curve_t &curve, uint8_t &level)
{
  uint8_t n = curve.enabled_points;
  if (!curve.enabled || (n < 2U))
  {
    return false;
  }
  for (uint8_t i = 1U; i < n; i++)
  {
    if (isInRange(input, curve.item[i - 1U].val, curve.item[i].val))
    {
      float levelf = mapValue(input,
                              curve.item[i - 1U].val,
                              curve.item[i].val,
                              curve.item[i - 1U].level,
                              curve.item[i].level);
      level = roundf(levelf);
      return true;
    }
  }
  if (curve.item[0].val < curve.item[n - 1U].val)
  {
    if (input < curve.item[0].val)
    {
      level = curve.item[0].level;
    }
    else
    {
      level = curve.item[n - 1U].level;
    }
  }
  else
  {
    if (input < curve.item[n - 1U].val)
    {
      level = curve.item[n - 1U].level;
    }
    else
    {
      level = curve.item[0].level;
    }
  }
  return true;
}

bool calcFanLevelByHumidityCurve(uint8_t &fan_level)
{
  uint8_t new_level = fan_level;
  calcFanLevelCurve(state.humidity.abs_delta, settings.ctrl.humidity_fan_curve, new_level);
  bool active = reduceFanLevel(fan_level, new_level);
  state.ctrl_active.humidity_fan_curve = active ? new_level : settings.ctrl.fan_level_max;
  return active;
}

bool calcFanLevelByTempCurve(uint8_t &fan_level)
{
  uint8_t new_level = fan_level;
  calcFanLevelCurve(state.temp.fresh_out, settings.ctrl.temp_fan_curve, new_level);
  bool active = reduceFanLevel(fan_level, new_level);
  state.ctrl_active.temp_fan_curve = active ? new_level : settings.ctrl.fan_level_max;
  return active;
}

uint8_t calcFanFrostLevel()
{
  uint8_t fan_level = FAN_LEVEL_OFF;
  if (state.actuator.open_flap_frost > FLAP_LEVEL_CLOSE)
  {
    uint8_t new_level = FAN_LEVEL_OFF;
    calcFanLevelCurve(state.temp.fresh_in, settings.ctrl.frost_fan_curve, new_level);
    bool active = increaseFanLevel(fan_level, new_level);
    state.ctrl_active.frost_fan_curve = active ? new_level : settings.ctrl.fan_frost_level_min;
  }
  return fan_level;
}
