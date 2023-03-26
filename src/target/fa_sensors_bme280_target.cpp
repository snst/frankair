// Copyright (c) 2023 by Stefan Schmidt
#include "fa_sensors_bme280_target.h"
#include "fa_controller.h"
#include "fa_log.h"
#include "fa_error.h"

#include <Wire.h>
#include "SparkFunBME280.h"

BME280 bmeF;
BME280 bmeE;

void sensorsBME280Setup()
{
    Wire.begin();

    bmeE.setI2CAddress(0x76U);
    bmeF.setI2CAddress(0x77U);

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
    BME280_SensorMeasurements me = {0U};
    BME280_SensorMeasurements mf = {0U};
    if (bmeE.readAllMeasurements(&me))
    {
        filterValue(state_raw.temp.exhaust_in, me.temperature);
        filterValue(state_raw.humidity.rel_exhaust_in, me.humidity);
    }
    else
    {
        IMSG(LERROR, "Error: BME exhaust IN failed.");
        errorSet(ERROR_SENSOR_EXHAUST_IN);
    }
    if (bmeF.readAllMeasurements(&mf))
    {
        filterValue(state_raw.temp.fresh_out, mf.temperature);
        filterValue(state_raw.humidity.rel_fresh_out, mf.humidity);
    }
    else
    {
        IMSG(LERROR, "Error: BME fresh OUT failed.");
        errorSet(ERROR_SENSOR_FRESH_OUT);
    }
}
