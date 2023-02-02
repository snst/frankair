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
    if (settings.use_calibration_temp)
    {
        correctTempWithCalibrationData(state.temp, state_raw.temp);
    }
    else
    {
        state.temp.exhaust_in = state_raw.temp.exhaust_in;
        state.temp.exhaust_out = state_raw.temp.exhaust_out;
        state.temp.fresh_in = state_raw.temp.fresh_in;
        state.temp.fresh_out = state_raw.temp.fresh_out;
    }

    if (settings.use_calibration_humidity)
    {
        correctHumidityWithCalibrationData(state.humidity, state_raw.humidity);
    }
    else
    {
        state.humidity.rel_exhaust_in = state_raw.humidity.rel_exhaust_in;
        state.humidity.rel_fresh_out = state_raw.humidity.rel_fresh_out;
    }

    if (settings.use_calibrated_temp_for_abs_humidity)
    {
        state.humidity.abs_exhaust_in = convertRelativeToAbsoluteHumidity(state.temp.exhaust_in, state.humidity.rel_exhaust_in);
        state.humidity.abs_fresh_out = convertRelativeToAbsoluteHumidity(state.temp.fresh_out, state.humidity.rel_fresh_out);
    }
    else
    {
        state.humidity.abs_exhaust_in = convertRelativeToAbsoluteHumidity(state_raw.temp.exhaust_in, state.humidity.rel_exhaust_in);
        state.humidity.abs_fresh_out = convertRelativeToAbsoluteHumidity(state_raw.temp.fresh_out, state.humidity.rel_fresh_out);
    }
    state.humidity.abs_delta = state.humidity.abs_exhaust_in - state.humidity.abs_fresh_out;

    logTempHumidity(LSENSOR, override.enabled ? "SIM" : "RAW", state_raw.temp, state_raw.humidity);
    logTempHumidity(LSENSOR, "FIL", state.temp, state.humidity);
}
