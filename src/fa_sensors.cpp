// Copyright (c) 2022 by Stefan Schmidt
#include "fa_sensors.h"

#include <OneWire.h>
#include <Wire.h>
#include <DallasTemperature.h>
#include "BlueDot_BME280.h"

#include "fa_settings.h"
#include "fa_controller.h"
#include "fa_calc.h"

OneWire oneWire(GPIO_ONE_WIRE);

BlueDot_BME280 bmeF;
BlueDot_BME280 bmeE;
DallasTemperature dallas(&oneWire);

static uint32_t sensors_now = 0;
static bool s_flag_scan_sensors = false;
static bool s_flag_bmeF_init = false;
static bool s_flag_bmeE_init = false;

DeviceAddress sensor_fresh_in = {0x28, 0xFF, 0x64, 0x01, 0xB9, 0xD9, 0x54, 0xAB};
DeviceAddress sensor_exhaust_out = {0x28, 0xFF, 0x64, 0x01, 0xB9, 0xBB, 0x64, 0xDE};
// DeviceAddress sensor_fresh_out =    { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
// DeviceAddress sensor_exhaust_in =   { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

void sensors_scan_intern();

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
  bmeE.parameter.sensorMode = 0b11;

  // The IIR (Infinite Impulse Response) filter suppresses high frequency fluctuations
  // In short, a high factor value means less noise, but measurements are also less responsive
  // You can play with these values and check the results!
  // In doubt just leave on default
  // 0b000:      factor 0 (filter off)
  // 0b001:      factor 2
  // 0b010:      factor 4
  // 0b011:      factor 8
  // 0b100:      factor 16 (default value)
  bmeF.parameter.IIRfilter = 0b100;
  bmeE.parameter.IIRfilter = 0b100;

  // Next you'll define the oversampling factor for the humidity measurements
  // Again, higher values mean less noise, but slower responses
  // If you don't want to measure humidity, set the oversampling to zero
  // 0b000:      factor 0 (Disable humidity measurement)
  // 0b001:      factor 1
  // 0b010:      factor 2
  // 0b011:      factor 4
  // 0b100:      factor 8
  // 0b101:      factor 16 (default value)
  bmeF.parameter.humidOversampling = 0b101;
  bmeE.parameter.humidOversampling = 0b101;

  // Now define the oversampling factor for the temperature measurements
  // You know now, higher values lead to less noise but slower measurements
  // 0b000:      factor 0 (Disable temperature measurement)
  // 0b001:      factor 1
  // 0b010:      factor 2
  // 0b011:      factor 4
  // 0b100:      factor 8
  // 0b101:      factor 16 (default value)
  bmeF.parameter.tempOversampling = 0b101;
  bmeE.parameter.tempOversampling = 0b101;

  // Finally, define the oversampling factor for the pressure measurements
  // For altitude measurements a higher factor provides more stable values
  // On doubt, just leave it on default
  // 0b000:      factor 0 (Disable pressure measurement)
  // 0b001:      factor 1
  // 0b010:      factor 2
  // 0b011:      factor 4
  // 0b100:      factor 8
  // 0b101:      factor 16 (default value)
  bmeF.parameter.pressOversampling = 0b101;
  bmeE.parameter.pressOversampling = 0b101;

  // For precise altitude measurements please put in the current pressure corrected for the sea level
  // On doubt, just leave the standard pressure as default (1013.25 hPa);
  bmeF.parameter.pressureSeaLevel = 1013.25;
  bmeE.parameter.pressureSeaLevel = 1013.25;

  // Also put in the current average temperature outside (yes, really outside!)
  // For slightly less precise altitude measurements, just leave the standard temperature as default (15°C and 59°F);
  bmeF.parameter.tempOutsideCelsius = 15;
  bmeE.parameter.tempOutsideCelsius = 15;
  bmeF.parameter.tempOutsideFahrenheit = 59;
  bmeE.parameter.tempOutsideFahrenheit = 59;
}

void sensors_setup()
{
  dallas.begin();
  bme_setup();
}

void filter_value(const char *txt, float &val, float measurement)
{
  val = (fa_settings.measurement_alpha * measurement) + ((1.0f - fa_settings.measurement_alpha) * val);
  IMSG(txt, measurement);
}

void read_bme(const char *name, BlueDot_BME280 &bme, bool &fInit, float &tempC, float &humR, float &humA)
{
  if (!fInit)
  {
    fInit = bme.init() == 0x60;
  }
  if (!fInit)
  {
    IMSG(name, "failed");
  }
  else
  {
    IMSG(name, "data");
    filter_value("TempC", tempC, bme.readTempC());
    filter_value("hum rel", humR, bme.readHumidity());
    filter_value("hum abs", humA, convertRelativeToAbsoluteHumidity(tempC, humR));
  }
}

void sensors_read()
{
  IMSG("sensors_read()\n");

  dallas.requestTemperatures();
  filter_value("dallas1", fa_state.temp_fresh_in, dallas.getTempC(sensor_fresh_in));
  filter_value("dallas2", fa_state.temp_exhaust_out, dallas.getTempC(sensor_exhaust_out));

  read_bme("bme_fresh", bmeF, s_flag_bmeF_init, fa_state.temp_fresh_out, fa_state.humidity_rel_fresh_out, fa_state.humidity_abs_fresh_out);
  read_bme("bme_exhaust", bmeE, s_flag_bmeE_init, fa_state.temp_exhaust_in, fa_state.humidity_rel_exaust_in, fa_state.humidity_abs_exaust_in);
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
