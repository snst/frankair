// Copyright (c) 2023 by Stefan Schmidt
#define THINGER_SERIAL_DEBUG

#include "fa_sm_actions.h"
#include "fa_fan.h"
#include "fa_flap.h"
#include "fa_controller.h"
#include "fa_error.h"

void smActionModeManual()
{
  IMSG(LSM, __func__);
  fanSetLevelFresh(settings.manual.level_fan_fresh);
  fanSetLevelExhaust(settings.manual.level_fan_exhaust);
  fanSetLevelFrost(settings.manual.level_fan_frost);
  flapSetOpen(settings.manual.open_flap_frost);
}

void smActionModeOff()
{
  IMSG(LSM, __func__);
  fanSetMainOff();
  fanSetFrostOff();
  flapClose();
}

void smActionModeSniff()
{
  IMSG(LSM, __func__);
  fanSetMainSniffing();
  fanSetFrostOff();
  flapClose();
}

void smActionModeWait()
{
  IMSG(LSM, __func__);
  fanSetMainWaiting();
  fanSetFrostOff();
  flapClose();
}

void smActionModeOnMax()
{
  IMSG(LSM, __func__);
  fanSetLevelMainWithOffset(settings.ctrl.fan_level_max);
}

void smActionModeOnAdjustMainFan()
{
  uint8_t fan_level = settings.ctrl.fan_level_max;
  if (settings.ctrl.humidity_fan_curve.enabled)
  {
    calcFanLevelByHumidityCurve(fan_level);
  }
  if (settings.ctrl.temp_fan_curve.enabled)
  {
    calcFanLevelByTempCurve(fan_level);
  }
  //IMSG(LSM, __func__, fan_level);
  fanSetLevelMainWithOffset(fan_level);
}

void smActionModeOnAdjustFrost()
{
  uint8_t fan_level;
  if (settings.ctrl.frost_flap_ctrl.enabled)
  {
    float temp = state.temp.fresh_in;
    if (temp <= settings.ctrl.frost_flap_ctrl.temp_min_open)
    {
      flapOpen();
    }
    if (temp >= settings.ctrl.frost_flap_ctrl.temp_min_close)
    {
      flapClose();
    }

    fan_level = calcFanFrostLevel();
    fanSetLevelFrost(fan_level);
  }
  else
  {
    flapSetOpen(settings.manual.open_flap_frost);
    fan_level = settings.manual.level_fan_frost;
    fanSetLevelFrost(fan_level);
  }
  //IMSG(LSM, __func__, fan_level);
}

void smActionReboot()
{
  IMSG(LSM, __func__);
  rebootTarget();
}