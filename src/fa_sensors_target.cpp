// Copyright (c) 2023 by Stefan Schmidt
#include "fa_sensors.h"

#include <OneWire.h>
#include <Wire.h>
#include <DallasTemperature.h>
#include "BlueDot_BME280.h"

#include "fa_settings.h"
#include "fa_controller.h"
#include "fa_calc.h"
#include "fa_calibration.h"

OneWire oneWire(GPIO_ONE_WIRE);

BlueDot_BME280 bmeF;
BlueDot_BME280 bmeE;
DallasTemperature dallas(&oneWire);

static bool s_flag_bmeF_init = false;
static bool s_flag_bmeE_init = false;

DeviceAddress sensor_fresh_in = {0x28, 0xFF, 0x64, 0x01, 0xB9, 0xD9, 0x54, 0xAB};
DeviceAddress sensor_exhaust_out = {0x28, 0xFF, 0x64, 0x01, 0xB9, 0xBB, 0x64, 0xDE};
// DeviceAddress sensor_fresh_out =    { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
// DeviceAddress sensor_exhaust_in =   { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

void bme_setup()
{
  bmeF.parameter.communication = 0;
  bmeE.parameter.communication = 0;
  bmeF.parameter.I2CAddress = 0x77;
  bmeE.parameter.I2CAddress = 0x76;

  // 0b00:     In sleep mode no measurements are performed, but power consumption is at a minimum
  // 0b01:     In forced mode a single measured is performed and the device returns automatically to sleep mode
  // 0b11:     In normal mode the sensor measures continually (default value)
  bmeF.parameter.sensorMode = 0b11;
  bmeE.parameter.sensorMode = bmeF.parameter.sensorMode;

  // The IIR (Infinite Impulse Response) filter suppresses high frequency fluctuations
  // In short, a high factor value means less noise, but measurements are also less responsive
  // You can play with these values and check the results!
  // In doubt just leave on default
  // 0b000:      factor 0 (filter off)
  // 0b001:      factor 2
  // 0b010:      factor 4
  // 0b011:      factor 8
  // 0b100:      factor 16 (default value)
  bmeF.parameter.IIRfilter = 0b010;
  bmeE.parameter.IIRfilter = bmeF.parameter.IIRfilter;

  // Next you'll define the oversampling factor for the humidity measurements
  // Again, higher values mean less noise, but slower responses
  // If you don't want to measure humidity, set the oversampling to zero
  // 0b000:      factor 0 (Disable humidity measurement)
  // 0b001:      factor 1
  // 0b010:      factor 2
  // 0b011:      factor 4
  // 0b100:      factor 8
  // 0b101:      factor 16 (default value)
  bmeF.parameter.humidOversampling = 0b011;
  bmeE.parameter.humidOversampling = bmeF.parameter.humidOversampling;

  // Now define the oversampling factor for the temperature measurements
  // You know now, higher values lead to less noise but slower measurements
  // 0b000:      factor 0 (Disable temperature measurement)
  // 0b001:      factor 1
  // 0b010:      factor 2
  // 0b011:      factor 4
  // 0b100:      factor 8
  // 0b101:      factor 16 (default value)
  bmeF.parameter.tempOversampling = 0b011;
  bmeE.parameter.tempOversampling = bmeF.parameter.tempOversampling;

  // Finally, define the oversampling factor for the pressure measurements
  // For altitude measurements a higher factor provides more stable values
  // On doubt, just leave it on default
  // 0b000:      factor 0 (Disable pressure measurement)
  // 0b001:      factor 1
  // 0b010:      factor 2
  // 0b011:      factor 4
  // 0b100:      factor 8
  // 0b101:      factor 16 (default value)
  bmeF.parameter.pressOversampling = 0b000;
  bmeE.parameter.pressOversampling = bmeF.parameter.pressOversampling;

  // For precise altitude measurements please put in the current pressure corrected for the sea level
  // On doubt, just leave the standard pressure as default (1013.25 hPa);
  bmeF.parameter.pressureSeaLevel = 1013.25;
  bmeE.parameter.pressureSeaLevel = bmeF.parameter.pressureSeaLevel;

  // Also put in the current average temperature outside (yes, really outside!)
  // For slightly less precise altitude measurements, just leave the standard temperature as default (15°C and 59°F);
  bmeF.parameter.tempOutsideCelsius = 15;
  bmeE.parameter.tempOutsideCelsius = bmeF.parameter.tempOutsideCelsius;
  bmeF.parameter.tempOutsideFahrenheit = 59;
  bmeE.parameter.tempOutsideFahrenheit = bmeF.parameter.tempOutsideFahrenheit;
}

void sensors_setup()
{
  dallas.begin();
  bme_setup();
}

void read_bme(const char *name, BlueDot_BME280 &bme, bool &fInit, float &tempC, float &humR)
{
  if (!fInit)
  {
    fInit = bme.init();
  }
  if (!fInit)
  {
    IMSG(LM_SENSOR, name, "failed");
  }
  else
  {
    IMSG(LM_SENSOR, name, "ok");
    tempC = bme.readTempC();
    humR = bme.readHumidity();
  }
}

void sensorsRead()
{
  IMSG(LM_SENSOR, "*** sensorsRead() ***");

  dallas.requestTemperatures();
  fa_state_raw.temp.fresh_in = dallas.getTempC(sensor_fresh_in);
  fa_state_raw.temp.exhaust_out = dallas.getTempC(sensor_exhaust_out);

  read_bme("bme_fresh_out", bmeF, s_flag_bmeF_init, fa_state_raw.temp.fresh_out, fa_state_raw.humidity.rel_fresh_out);
  read_bme("bme_exhaust_in", bmeE, s_flag_bmeE_init, fa_state_raw.temp.exhaust_in, fa_state_raw.humidity.rel_exaust_in);
}

void sensors_scan_intern()
{
  uint8_t addr[8];

  if (!oneWire.search(addr))
  {
    IMSG(LM_SENSOR, "No more onewire addresses.");
    oneWire.reset_search();
    s_flag_scan_sensors = false;
  }
  else
  {
    IMSG(LM_SENSOR, "addr=");
    for (uint8_t i = 0; i < sizeof(addr); i++)
    {
      IMSG(LM_SENSOR, "0x");
      IMSGHEX(LM_SENSOR, addr[i]);
    }
  }
}
