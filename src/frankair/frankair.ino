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

OneWire oneWire(GPIO_ONE_WIRE);
TwoWire I2CBME = TwoWire(0);

 

float val_temp[MAX_TEMP_SENSORS] = {0};
float val_temp_interval = 1000;
uint8_t val_pwm = 0;
bool val_enabled = true;

uint8_t val_snooping_fan_level = 5;
uint8_t val_snooping_duration_sec = 60;
uint8_t val_snooping_interval_min = 30;
uint8_t val_measuring_interval_sec = 10;
uint8_t val_control_interval_sec = 60;
uint8_t val_state = 0;
uint8_t val_servo_frost = 0;
uint8_t val_fan_frost = 0;
uint8_t val_fan_exhaust_air = 0;
uint8_t val_fan_fresh_air = 0;

bool flag_scan_sensors = false;


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
