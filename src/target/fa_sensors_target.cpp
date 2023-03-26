// Copyright (c) 2023 by Stefan Schmidt
#include "fa_sensors_dallas_target.h"
#include "fa_sensors_bme280_target.h"
#include "fa_controller.h"
#include "fa_calc.h"

void sensorsSetupPlatform()
{
  sensorsDallasSetup();
  sensorsBME280Setup();
}

void sensorsRead()
{
  sensorsDallasRead();
  sensorsBME280Read();
}
