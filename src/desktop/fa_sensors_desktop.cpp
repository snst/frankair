// Copyright (c) 2023 by Stefan Schmidt
#include "fa_sensors.h"
#include "fa_error.h"

uint8_t desktop_sensor_error = 0U;

void sensorsRead()
{
    filterValue(state_raw.temp.fresh_in, override.temp.fresh_in);
    filterValue(state_raw.temp.exhaust_out, override.temp.exhaust_out);
    filterValue(state_raw.temp.exhaust_in, override.temp.exhaust_in);
    filterValue(state_raw.humidity.rel_exhaust_in, override.humidity_rel_exhaust_in);
    filterValue(state_raw.temp.fresh_out, override.temp.fresh_out);
    filterValue(state_raw.humidity.rel_fresh_out, override.humidity_rel_fresh_out);
    if (desktop_sensor_error) {
        errorSet(desktop_sensor_error);
    }
}

void sensorsSetupPlatform()
{
}
