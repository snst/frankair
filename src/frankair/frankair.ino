// Copyright (c) 2022 by Stefan Schmidt
#define THINGER_SERIAL_DEBUG

#include <Wire.h>
#include <OneWire.h>

#include "fa_log.h"
#include "fa_defines.h"
#include "fa_common.h"
#include "fa_thing.h"
#include "fa_settings.h"
#include "fa_sensors.h"
#include "fa_controller.h"
#include "fa_fan.h"
#include "fa_servo.h"

OneWire oneWire(GPIO_ONE_WIRE);
TwoWire I2CBME = TwoWire(0);

void setup()
{
  logging_setup();
  IMSG("Welcome to frankair!\n");

  settings_load();
  fan_setup();
  sensors_setup();
  servo_setup();
  thing_setup();
}

void loop()
{
  interval_update();
  thing.handle();
  sensors_update();
  controller_update();
}
