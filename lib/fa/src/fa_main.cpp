// Copyright (c) 2023 by Stefan Schmidt
#define THINGER_SERIAL_DEBUG

#include "fa_common.h"
#include "fa_log.h"
#include "fa_defines.h"
#include "fa_thing.h"
#include "fa_settings.h"
#include "fa_sensors.h"
#include "fa_controller.h"
#include "fa_fan.h"
#include "fa_flap.h"
#include "fa_led.h"

void setup()
{
  logging_setup();
  IMSG(LM_COMMON, "Welcome to frankair!");
  calibration_load();
  settings_load();
  led_setup();
  fan_setup();
  sensors_setup();
  flap_setup();
  thing_setup();
}

void loop()
{
  interval_update();
  thing_update();
  sensors_update();
  controller_update();
  led_update();
}
