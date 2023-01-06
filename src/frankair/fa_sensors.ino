// Copyright (c) 2022 by Stefan Schmidt
#include "fa_sensors.h"
#include "fa_defines.h"
#include "fa_controller.h"

#include <OneWire.h>
#include <Wire.h>
#include <DallasTemperature.h>
#include "BlueDot_BME280.h"

extern OneWire oneWire;
//extern TwoWire i2c;


BlueDot_BME280 bme1;
BlueDot_BME280 bme2;
DallasTemperature dallas(&oneWire);

static uint32_t sensors_now = 0;
static bool s_flag_scan_sensors = false;
static bool s_flag_bme1_init = false;
static bool s_flag_bme2_init = false;

DeviceAddress sensor_fresh_in = {0x28, 0xFF, 0x64, 0x01, 0xB9, 0xD9, 0x54, 0xAB};
DeviceAddress sensor_exhaust_out = {0x28, 0xFF, 0x64, 0x01, 0xB9, 0xBB, 0x64, 0xDE};
// DeviceAddress sensor_fresh_out =    { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
// DeviceAddress sensor_exhaust_in =   { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

void sensors_scan_intern();

void bme_setup()
{
  bme1.parameter.communication = 0;
  bme2.parameter.communication = 0;
  bme1.parameter.I2CAddress = 0x77;
  bme2.parameter.I2CAddress = 0x76;

  //0b00:     In sleep mode no measurements are performed, but power consumption is at a minimum
  //0b01:     In forced mode a single measured is performed and the device returns automatically to sleep mode
  //0b11:     In normal mode the sensor measures continually (default value)
  bme1.parameter.sensorMode = 0b11;
  bme2.parameter.sensorMode = 0b11; 

  //The IIR (Infinite Impulse Response) filter suppresses high frequency fluctuations
  //In short, a high factor value means less noise, but measurements are also less responsive
  //You can play with these values and check the results!
  //In doubt just leave on default
  //0b000:      factor 0 (filter off)
  //0b001:      factor 2
  //0b010:      factor 4
  //0b011:      factor 8
  //0b100:      factor 16 (default value)
  bme1.parameter.IIRfilter = 0b100;
  bme2.parameter.IIRfilter = 0b100;
  
  //Next you'll define the oversampling factor for the humidity measurements
  //Again, higher values mean less noise, but slower responses
  //If you don't want to measure humidity, set the oversampling to zero
  //0b000:      factor 0 (Disable humidity measurement)
  //0b001:      factor 1
  //0b010:      factor 2
  //0b011:      factor 4
  //0b100:      factor 8
  //0b101:      factor 16 (default value)
  bme1.parameter.humidOversampling = 0b101;
  bme2.parameter.humidOversampling = 0b101;

  //Now define the oversampling factor for the temperature measurements
  //You know now, higher values lead to less noise but slower measurements
  //0b000:      factor 0 (Disable temperature measurement)
  //0b001:      factor 1
  //0b010:      factor 2
  //0b011:      factor 4
  //0b100:      factor 8
  //0b101:      factor 16 (default value)
  bme1.parameter.tempOversampling = 0b101;
  bme2.parameter.tempOversampling = 0b101;

  //Finally, define the oversampling factor for the pressure measurements
  //For altitude measurements a higher factor provides more stable values
  //On doubt, just leave it on default
  //0b000:      factor 0 (Disable pressure measurement)
  //0b001:      factor 1
  //0b010:      factor 2
  //0b011:      factor 4
  //0b100:      factor 8
  //0b101:      factor 16 (default value)  
  bme1.parameter.pressOversampling = 0b101;
  bme2.parameter.pressOversampling = 0b101;

  //For precise altitude measurements please put in the current pressure corrected for the sea level
  //On doubt, just leave the standard pressure as default (1013.25 hPa);  
  bme1.parameter.pressureSeaLevel = 1013.25;
  bme2.parameter.pressureSeaLevel = 1013.25;

  //Also put in the current average temperature outside (yes, really outside!)
  //For slightly less precise altitude measurements, just leave the standard temperature as default (15°C and 59°F);
  bme1.parameter.tempOutsideCelsius = 15;
  bme2.parameter.tempOutsideCelsius = 15;
  bme1.parameter.tempOutsideFahrenheit = 59;
  bme2.parameter.tempOutsideFahrenheit = 59;
}

bool bme1_setup()
{
  if (!s_flag_bme1_init) {
    s_flag_bme1_init = bme1.init() == 0x60;
  }
  IMSG("bme1_setup", s_flag_bme1_init);
  return s_flag_bme1_init;
}

bool bme2_setup()
{
  if (!s_flag_bme2_init) {
    s_flag_bme2_init = bme2.init() == 0x60;
  }
  IMSG("bme2_setup", s_flag_bme2_init);
  return s_flag_bme2_init;
}

void sensors_setup()
{
  dallas.begin();
  //i2c.begin(I2C_SDA, I2C_SCL, 100000); 
  bme_setup();
  bme1_setup();
  bme2_setup();
  // int sensorCount = dallas.getDS18Count();
}

void filter_value(const char* txt, float &val, float measurement)
{
  val = (fa_settings.measurement_alpha * measurement) + ((1.0f - fa_settings.measurement_alpha) * val);
  IMSG(txt, measurement);
}



void sensors_read()
{
  IMSG("sensors_read()\n");
  dallas.requestTemperatures();
  filter_value("dallas1", fa_state.temp_fresh_in, dallas.getTempC(sensor_fresh_in));
  filter_value("dallas2", fa_state.temp_exhaust_out, dallas.getTempC(sensor_exhaust_out));

  if (bme1_setup()) 
  {
    filter_value("bme1 temp", fa_state.temp_fresh_out, bme1.readTempC());
    filter_value("bme1 hum", fa_state.humidity_rel_fresh_out, bme1.readHumidity());
  }
  if (bme2_setup()) 
  {
    filter_value("bme2 temp", fa_state.temp_exhaust_in, bme2.readTempC());
    filter_value("bme2 hum", fa_state.humidity_rel_exaust_in, bme2.readHumidity());
  }
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

// https://github.com/finitespace/BME280/blob/master/src/EnvironmentCalculations.cpp
float EnvironmentCalculations::AbsoluteHumidity
(
  float temperature, 
  float humidity,
  TempUnit tempUnit
)
{
  //taken from https://carnotcycle.wordpress.com/2012/08/04/how-to-convert-relative-humidity-to-absolute-humidity/
  //precision is about 0.1°C in range -30 to 35°C
  //August-Roche-Magnus 	6.1094 exp(17.625 x T)/(T + 243.04)
  //Buck (1981) 		6.1121 exp(17.502 x T)/(T + 240.97)
  //reference https://www.eas.ualberta.ca/jdwilson/EAS372_13/Vomel_CIRES_satvpformulae.html
  float temp = NAN;
  const float mw = 18.01534; 	// molar mass of water g/mol
  const float r = 8.31447215; 	// Universal gas constant J/mol/K

  if (isnan(temperature) || isnan(humidity) )
  {
    return NAN;
  }
  
  if(tempUnit != TempUnit_Celsius)
  {
        temperature = (temperature - 32.0) * (5.0 / 9.0); /*conversion to [°C]*/
  }
          
  temp = pow(2.718281828, (17.67 * temperature) / (temperature + 243.5));
  
  //return (6.112 * temp * humidity * 2.1674) / (273.15 + temperature); 	//simplified version
  return (6.112 * temp * humidity * mw) / ((273.15 + temperature) * r); 	//long version
}