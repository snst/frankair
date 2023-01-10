// Copyright (c) 2023 by Stefan Schmidt
#include "fa_calc.h"
#include <math.h>
#include <stdio.h>
#include "fa_settings.h"
#include "fa_log.h"
#include "fa_calibration.h"

extern fa_state_t fa_state;
extern fa_state_raw_t fa_state_raw;

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

void filterValue(const char *txt, float &val, float measurement)
{
    val = (fa_settings.measurement_alpha * measurement) + ((1.0f - fa_settings.measurement_alpha) * val);
    IMSG(LM_SENSOR, txt, measurement);
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

    if (fa_settings.use_calibration)
    {
        IMSG(LM_SENSOR, "*** sensorsProcessValues(CALIBRATION) ***");
        correctTempWithCalibrationData(temp, fa_state_raw.temp);
        correctHumidityWithCalibrationData(humidity, fa_state_raw.humidity);
    }
    else
    {
        IMSG(LM_SENSOR, "*** sensorsProcessValues(RAW) ***");
        temp.exhaust_in = fa_state_raw.temp.exhaust_in;
        temp.exhaust_out = fa_state_raw.temp.exhaust_out;
        temp.fresh_in = fa_state_raw.temp.fresh_in;
        temp.fresh_out = fa_state_raw.temp.fresh_out;
        humidity.rel_exaust_in = fa_state_raw.humidity.rel_exaust_in;
        humidity.rel_fresh_out = fa_state_raw.humidity.rel_fresh_out;
    }

    filterValue("temp_exhaust_in", fa_state.temp.exhaust_in, temp.exhaust_in);
    filterValue("temp_exhaust_out", fa_state.temp.exhaust_out, temp.exhaust_out);
    filterValue("temp_fresh_in", fa_state.temp.fresh_in, temp.fresh_in);
    filterValue("temp_fresh_out", fa_state.temp.fresh_out, temp.fresh_out);
    filterValue("hum_rel_exaust_in", fa_state.humidity.rel_exaust_in, humidity.rel_exaust_in);
    filterValue("hum_rel_fresh_out", fa_state.humidity.rel_fresh_out, humidity.rel_fresh_out);
    fa_state.humidity.abs_exaust_in = convertRelativeToAbsoluteHumidity(fa_state.temp.exhaust_in, fa_state.humidity.rel_exaust_in);
    fa_state.humidity.abs_fresh_out = convertRelativeToAbsoluteHumidity(fa_state.temp.fresh_out, fa_state.humidity.rel_fresh_out);
    fa_state.humidity.abs_delta = fa_state.humidity.abs_exaust_in - fa_state.humidity.abs_fresh_out;
}