// Copyright (c) 2023 by Stefan Schmidt
#include "fa_fan.h"
#include "fa_log.h"
#include "fa_calibration.h"
#include "fa_controller.h"
#include "fa_common.h"

static uint8_t last_exhaust_level = 0xFF;
static uint8_t last_fresh_level = 0xFF;
static uint8_t last_frost_level = 0xFF;

void fanSetLevelMain(uint8_t gpio, uint8_t level)
{
  fanSetPWM(gpio, 255U - calibration_actuator.fan_pwm_main[level], calibration_actuator.fan_freq_main);
}

void fanSetLevelFresh(uint8_t level)
{
  level = adjustFanLevelToValidRange(level);
  if (updateIfChanged(last_fresh_level, level, g_update_stream))
  {
    fanSetLevelMain(GPIO_PWM1, level);
    IMSG(LACTUATOR, __func__, level);
  }
  state.actuator.level_fan_fresh = level;
}

void fanSetLevelExhaust(uint8_t level)
{
  level = adjustFanLevelToValidRange(level);
  if (updateIfChanged(last_exhaust_level, level, g_update_stream))
  {
    fanSetLevelMain(GPIO_PWM2, level);
    IMSG(LACTUATOR, __func__, level);
  }
  state.actuator.level_fan_exhaust = level;
}

void fanSetLevelFrost(uint8_t level)
{
  level = adjustFanLevelToValidRange(level);
  if (updateIfChanged(last_frost_level, level, g_update_stream))
  {
    fanSetPWM(GPIO_PWM3, calibration_actuator.fan_pwm_frost[level], calibration_actuator.fan_freq_frost);
    IMSG(LACTUATOR, __func__, level);
  }
  state.actuator.level_fan_frost = level;
}

void fanSetLevelMain(uint8_t level)
{
  fanSetLevelFresh(level);
  fanSetLevelExhaust(level);
}

void fanSetLevelMainWithOffset(uint8_t level)
{
  fanSetLevelFresh(toRange(((int8_t)level) + settings.ctrl.fan_offset_fresh, settings.ctrl.fan_level_min, settings.ctrl.fan_level_max));
  fanSetLevelExhaust(toRange(((int8_t)level) + settings.ctrl.fan_offset_exhaust, settings.ctrl.fan_level_min, settings.ctrl.fan_level_max));
}

void fanSetMainOff()
{
  fanSetLevelMain(FAN_LEVEL_OFF);
}

void fanSetMainMinimum()
{
  fanSetLevelMainWithOffset(settings.ctrl.fan_level_min);
}

void fanSetMainSniffing()
{
  fanSetLevelMainWithOffset(settings.sniff.fan_level_sniff);
}

void fanSetMainWaiting()
{
  fanSetLevelMainWithOffset(settings.sniff.fan_level_wait);
}

void fanSetFrostOff()
{
  fanSetLevelFrost(FAN_LEVEL_OFF);
}

void fanSetFrostMinimum()
{
  fanSetLevelFrost(settings.ctrl.fan_frost_level_min);
}