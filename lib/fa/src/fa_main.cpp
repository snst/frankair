// Copyright (c) 2023 by Stefan Schmidt
#define THINGER_SERIAL_DEBUG

#include "fa_common.h"
#include "fa_log.h"
#include "fa_defines.h"
#include "fa_thing.h"
#include "fa_settings.h"
#include "fa_sensors.h"
#include "fa_controller.h"
#include "fa_fan.h"
#include "fa_flap.h"
#include "fa_led.h"
#include "fa_ota.h"
#include "fa_version.h"

void setup()
{
  loggingSetup();
  IMSG(LM_COMMON, "Welcome to frankair! Version:", FA_VERSION);
  calibrationLoad();
  settingsLoad();
  ledSetup();
  fanSetup();
  sensorsSetup();
  flapSetup();
  thingSetup();
}

void loop()
{
  intervalUpdate();
  thingUpdate();
  sensorsUpdate();
  controllerUpdate();
  ledUpdate();
  otaUpdate();
}
