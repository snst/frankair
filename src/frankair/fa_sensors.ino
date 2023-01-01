// Copyright (c) 2022 by Stefan Schmidt
#include "fa_sensors.h"
#include "fa_defines.h"
#include "fa_controller.h"

#include <OneWire.h>
#include <Wire.h>
#include <DallasTemperature.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

extern OneWire oneWire;
extern TwoWire i2c;

// Adafruit_BMP280 bme(&i2c);

DallasTemperature dallas(&oneWire);

static uint32_t sensors_now = 0;
static bool s_flag_scan_sensors = false;

DeviceAddress sensor_fresh_in = {0x28, 0xFF, 0x64, 0x01, 0xB9, 0xD9, 0x54, 0xAB};
// DeviceAddress sensor_fresh_out =    { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
// DeviceAddress sensor_exhaust_in =   { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
DeviceAddress sensor_exhaust_out = {0x28, 0xFF, 0x64, 0x01, 0xB9, 0xBB, 0x64, 0xDE};

void sensors_scan_intern();

void sensors_setup()
{
  dallas.begin();
  // int sensorCount = dallas.getDS18Count();
}

void filter_value(float &val, float measurement)
{
  val = (fa_settings.measurement_alpha * measurement) + ((1.0f - fa_settings.measurement_alpha) * val);
}

void sensors_read()
{
  IMSG("sensors_read()\n");
  dallas.requestTemperatures();
  filter_value(fa_state.temp_fresh_in, dallas.getTempC(sensor_fresh_in));
  // filter_value(fa_state.temp_fresh_out, dallas.getTempC(sensor_fresh_out));
  // filter_value(fa_state.temp_exhaust_in, dallas.getTempC(sensor_exhaust_in));
  filter_value(fa_state.temp_exhaust_out, dallas.getTempC(sensor_exhaust_out));
}

void sensors_update()
{
  if (s_flag_scan_sensors)
  {
    sensors_scan_intern();
  }
  else if (interval(sensors_now, fa_settings.temp_sensor_read_interval_sec))
  {
    sensors_read();
  }
}

void sensors_scan()
{
  s_flag_scan_sensors = true;
}

void sensors_scan_intern()
{
  uint8_t addr[8];

  if (!oneWire.search(addr))
  {
    IMSG("\nNo more onewire addresses.\n");
    oneWire.reset_search();
    s_flag_scan_sensors = false;
  }
  else
  {
    IMSG("\naddr=");
    for (uint8_t i = 0; i < sizeof(addr); i++)
    {
      IMSG("0x");
      IMSGHEX(addr[i]);
      IMSG(", ");
    }
  }
}
