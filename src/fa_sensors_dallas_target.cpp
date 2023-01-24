// Copyright (c) 2023 by Stefan Schmidt
#include "fa_sensors_dallas_target.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include "fa_controller.h"
#include "fa_sensors.h"
#include "fa_defines.h"
#include "fa_common.h"
#include "fa_error.h"

OneWire oneWire(GPIO_ONE_WIRE);
DallasTemperature dallas(&oneWire);

DeviceAddress sensor_fresh_in = {0x28, 0xFF, 0x64, 0x01, 0xB9, 0xD9, 0x54, 0xAB};
DeviceAddress sensor_exhaust_out = {0x28, 0xFF, 0x64, 0x01, 0xB9, 0xBB, 0x64, 0xDE};

void sensorsDallasSetup()
{
  dallas.begin();
}

void sensorsDallasRead()
{
  dallas.requestTemperatures();

  float val = dallas.getTempC(sensor_fresh_in);
  if (DEVICE_DISCONNECTED_C == val)
  {
    IMSG(LERROR, "Error: Dallas fresh IN failed.");
    errorSet(ERROR_SENSOR_FRESH_IN);
  }
  else
  {
    state_raw.temp.fresh_in = val;
  }

  val = dallas.getTempC(sensor_exhaust_out);
  if (DEVICE_DISCONNECTED_C == val)
  {
    IMSG(LERROR, "Error: Dallas exhaust OUT failed.");
    errorSet(ERROR_SENSOR_EXHAUST_OUT);
  }
  else
  {
    state_raw.temp.exhaust_out = val;
  }
}

void sensorsDallasScan()
{
  uint8_t addr[8];

  if (!oneWire.search(addr))
  {
    IMSG(LSENSOR, "No more onewire addresses.");
    oneWire.reset_search();
  }
  else
  {
    IMSG(LSENSOR, "addr=");
    for (uint8_t i = 0; i < sizeof(addr); i++)
    {
      IMSG(LSENSOR, "0x");
      IMSGHEX(LSENSOR, addr[i]);
    }
  }
}
