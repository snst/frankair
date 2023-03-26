#include <stdint.h>
#include "fa_sm.h"
#include "fa_log.h"
#include "fa_settings.h"
#include <thread>
#include <iostream>
#include "fa_gui_desktop.h"

void setup();
void loop();

int main(int argc, char *argv[])
{
    setup();

    //override.enabled = true;
    settings.temp_sensor_read_interval_sec = 0;
    override.humidity_rel_exhaust_in = 60;
    override.humidity_rel_fresh_out = 50;
    override.temp.exhaust_in = 12.0f;
    override.temp.exhaust_out = 10.0f;
    override.temp.fresh_in = 6.0f;
    override.temp.fresh_out = 9.0f;
    /*
    state.humidity.abs_exhaust_in = 8;
    state.humidity.abs_fresh_out = 5;
*/
    GuiInit();

    while (!GuiShouldClose())
    {
        loop();
        GuiUpdate();
    }

    GuiCleanup();
}