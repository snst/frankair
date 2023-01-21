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
  fanSetPWM(gpio, 255U - calibration_actuator.fan_pwm_main[level]);
}

void fanSetLevelFresh(uint8_t level)
{
  level = adjustFanLevelToValidRange(level);
  if (updateIfChanged(last_fresh_level, level))
  {
    fanSetLevelMain(GPIO_PWM1, level);
    IMSG(LACTUATOR, "fanSetLevelFresh", level);
  }
  state.actuator.level_fan_fresh = level;
}

void fanSetLevelExhaust(uint8_t level)
{
  level = adjustFanLevelToValidRange(level);
  if (updateIfChanged(last_exhaust_level, level))
  {
    fanSetLevelMain(GPIO_PWM2, level);
    IMSG(LACTUATOR, "fanSetLevelExhaust", level);
  }
  state.actuator.level_fan_exhaust = level;
}

void fanSetLevelFrost(uint8_t level)
{
  level = adjustFanLevelToValidRange(level);
  if (updateIfChanged(last_frost_level, level))
  {
    fanSetPWM(GPIO_PWM3, calibration_actuator.fan_pwm_frost[level]);
    IMSG(LACTUATOR, "fanSetLevelFrost", level);
  }
  state.actuator.level_fan_frost = level;
}

void fanSetLevelFreshAndExhaust(uint8_t level)
{
  fanSetLevelFresh(level);
  fanSetLevelExhaust(level);
}
