// Copyright (c) 2023 by Stefan Schmidt
#include "fa_calc.h"
#include <math.h>
#include <stdio.h>
#include "fa_settings.h"
#include "fa_log.h"
#include "fa_calibration.h"
#include "fa_sensors.h"
#include "fa_structs.h"


float convertRelativeToAbsoluteHumidity(
    float temperature_C,
    float humidity)
{
    // taken from https://carnotcycle.wordpress.com/2012/08/04/how-to-convert-relative-humidity-to-absolute-humidity/
    // precision is about 0.1°C in range -30 to 35°C
    // August-Roche-Magnus 	6.1094 exp(17.625 x T)/(T + 243.04)
    // Buck (1981) 		6.1121 exp(17.502 x T)/(T + 240.97)
    // reference https://www.eas.ualberta.ca/jdwilson/EAS372_13/Vomel_CIRES_satvpformulae.html
    const float mw = 18.01534;  // molar mass of water g/mol
    const float r = 8.31447215; // Universal gas constant J/mol/K

    float temp = pow(2.718281828, (17.67 * temperature_C) / (temperature_C + 243.5));

    // return (6.112 * temp * humidity * 2.1674) / (273.15 + temperature_C); 	//simplified version
    float ret = (6.112 * temp * humidity * mw) / ((273.15 + temperature_C) * r); // long version
    return ret;
}

float calcAverage(float x1, float x2)
{
    return (x1 + x2) / 2.0f;
}

float calcAverage(float x1, float x2, float x3, float x4)
{
    return (x1 + x2 + x3 + x4) / 4.0f;
}

void filterValue(float &val, float measurement)
{
    val = (settings.measurement_alpha * measurement) + ((1.0f - settings.measurement_alpha) * val);
}

float calcCorrectedValue(float rawV, float rawMin, float rawMax, float refMin, float refMax)
{
    float ret = 0.0f;
    float rawRange = rawMax - rawMin;
    float refRange = refMax - refMin;
    if (0.0f != rawRange)
    {
        ret = (((rawV - rawMin) * refRange) / rawRange) + refMin;
    }
    return ret;
}

void sensorsProcessValues()
{
    fa_temp_t temp;
    fa_humidity_t humidity;

    if (settings.use_calibration_temp)
    {
        correctTempWithCalibrationData(temp, state_raw.temp);
    }
    else
    {
        temp.exhaust_in = state_raw.temp.exhaust_in;
        temp.exhaust_out = state_raw.temp.exhaust_out;
        temp.fresh_in = state_raw.temp.fresh_in;
        temp.fresh_out = state_raw.temp.fresh_out;
    }

    if (settings.use_calibration_humidity)
    {
        correctHumidityWithCalibrationData(humidity, state_raw.humidity);
    }
    else
    {
        humidity.rel_exhaust_in = state_raw.humidity.rel_exhaust_in;
        humidity.rel_fresh_out = state_raw.humidity.rel_fresh_out;
    }

    filterValue(state.temp.exhaust_in, temp.exhaust_in);
    filterValue(state.temp.exhaust_out, temp.exhaust_out);
    filterValue(state.temp.fresh_in, temp.fresh_in);
    filterValue(state.temp.fresh_out, temp.fresh_out);
    filterValue(state.humidity.rel_exhaust_in, humidity.rel_exhaust_in);
    filterValue(state.humidity.rel_fresh_out, humidity.rel_fresh_out);
    state.humidity.abs_exhaust_in = convertRelativeToAbsoluteHumidity(state.temp.exhaust_in, state.humidity.rel_exhaust_in);
    state.humidity.abs_fresh_out = convertRelativeToAbsoluteHumidity(state.temp.fresh_out, state.humidity.rel_fresh_out);
    state.humidity.abs_delta = state.humidity.abs_exhaust_in - state.humidity.abs_fresh_out;

    logTempHumidity(LSENSOR, override.enabled ? "SIM" : "RAW", state_raw.temp, state_raw.humidity);
    logTempHumidity(LSENSOR, "FIL", state.temp, state.humidity);
}
