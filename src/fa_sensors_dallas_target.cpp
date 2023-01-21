// Copyright (c) 2023 by Stefan Schmidt
#include "fa_sensors_dallas_target.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include "fa_controller.h"
#include "fa_sensors.h"

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
  state_raw.temp.fresh_in = dallas.getTempC(sensor_fresh_in);
  state_raw.temp.exhaust_out = dallas.getTempC(sensor_exhaust_out);
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
