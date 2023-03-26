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
#include "fa_statistic.h"
#include "fa_sm.h"

void setup()
{
  loggingSetup();
  IMSG(LINFO, "Welcome to frankair! Version:", FA_VERSION);
  settingsLoad();
  calibrationLoad();
  ledSetup();
  fanSetup();
  sensorsSetup();
  flapSetup();
  statisticSetup();
  thingSetup();
  smInit();
}

void loop()
{
  nowUpdate();
  thingUpdate();
  sensorsUpdate();
  smUpdate();
  ledUpdate();
  otaUpdate();
  thingUpdateStream();
}
