// Copyright (c) 2023 by Stefan Schmidt
#include "fa_sensors_bme280_target.h"
#include "fa_controller.h"
#include "fa_log.h"

#include <Wire.h>
#include "SparkFunBME280.h"

BME280 bmeF;
BME280 bmeE;

void sensorsBME280Setup()
{
    Wire.begin();

    bmeE.setI2CAddress(0x76);
    bmeF.setI2CAddress(0x77);

    if (!bmeE.beginI2C())
    {
        IMSG(LERROR, "Error: bmeE i2c");
    }
    IMSG(LDEBUG, "bmeE.getMode", bmeE.getMode());

    if (!bmeF.beginI2C())
    {
        IMSG(LERROR, "Error: bmeF i2c");
    }
    IMSG(LDEBUG, "bmeF.getMode", bmeF.getMode());
    
}

void sensorsBME280Read()
{
    bmeE.setMode(MODE_FORCED);
    bmeF.setMode(MODE_FORCED);
    BME280_SensorMeasurements me = {0};
    BME280_SensorMeasurements mf = {0};
    bmeE.readAllMeasurements(&me);
    bmeF.readAllMeasurements(&mf);
    state_raw.temp.exhaust_in = me.temperature;
    state_raw.humidity.rel_exaust_in = me.humidity;
    state_raw.temp.fresh_out = mf.temperature;
    state_raw.humidity.rel_fresh_out = mf.humidity;
}
