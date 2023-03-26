// Copyright (c) 2023 by Stefan Schmidt
#include "fa_sm.h"
#include "fa_common.h"



void cmdStartSniffing()
{
  smSendEvent(fa_sm_gen_EventId::fa_sm_gen_EventId_EV_AUTO_SNIFF);
}

void cmdStopSniffing()
{
  smSendEvent(fa_sm_gen_EventId::fa_sm_gen_EventId_EV_SNIFF_FINISHED);
}

void cmdSettingsChanged()
{
  showCmdFeedback();
  smSendEvent(fa_sm_gen_EventId::fa_sm_gen_EventId_EV_SETTINGS_CHANGED);
}

void cmdReboot()
{
  smSendEvent(fa_sm_gen_EventId::fa_sm_gen_EventId_EV_REBOOT, 5000U);
}
