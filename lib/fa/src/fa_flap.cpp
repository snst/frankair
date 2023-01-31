// Copyright (c) 2023 by Stefan Schmidt
#include "fa_defines.h"

#include "fa_flap.h"
#include "fa_settings.h"
#include "fa_calibration.h"
#include "fa_common.h"
#include "fa_controller.h"

static uint8_t last_flap_level = 0xFF;

void flapSetOpen(uint8_t level)
{
    uint8_t new_level = toRange(level, FLAP_LEVEL_MIN, FLAP_LEVEL_MAX);
    if (updateIfChanged(last_flap_level, new_level))
    {
        flapSetOpenPWM(
            mapValue(new_level,
                     FLAP_LEVEL_MIN, FLAP_LEVEL_MAX,
                     calibration_actuator.flap_pos.min,
                     calibration_actuator.flap_pos.max));
        IMSG(LACTUATOR, "flapSetOpen", new_level);
    }
    state.actuator.open_flap_frost = new_level;
}
