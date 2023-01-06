// Copyright (c) 2022 by Stefan Schmidt
#include "fa_calc.h"
#include <math.h>
#include <stdio.h>


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