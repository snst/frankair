// Copyright (c) 2023 by Stefan Schmidt
#define THINGER_SERIAL_DEBUG

#include "fa_sm.h"
#include "fa_fan.h"
#include "fa_flap.h"
#include "fa_thing.h"
#include "fa_statistic.h"
#include "fa_error.h"

fa_sm_gen g_state_machine;

void smInit()
{
  fa_sm_gen_ctor(&g_state_machine);
  g_state_machine.vars.settings = &settings;
  g_state_machine.vars.state = &state;
  fa_sm_gen_start(&g_state_machine);
}

void smUpdate()
{
  fa_sm_gen_EventId id;
  if (g_state_machine.vars.eventSM.update(id))
  {
    statisticUpdate();
    state.ctrl_active.decisions = 0U;
    fa_sm_gen_dispatch_event(&g_state_machine, id);
  }

  if ((state.sm_state != (uint8_t)g_state_machine.state_id))
  {
    state.sm_state = (uint8_t)g_state_machine.state_id;
    if (g_state_machine.state_id != fa_sm_gen_StateId_SNIFF)
    {
      thingSendStreamState();
    }
  }
}

fa_sm_gen_StateId smGetStateId()
{
  return g_state_machine.state_id;
}

void smSendEvent(enum fa_sm_gen_EventId event_id, uint32_t ms)
{
  g_state_machine.vars.eventSM.send(event_id, ms);
}

bool smInState(enum fa_sm_gen_StateId state)
{
  bool ret = (state == g_state_machine.state_id);
  return ret;
}

#if 0
const char *smGetStateStr()
{
  switch (g_state_machine.state_id)
  {
  case fa_sm_gen_StateId::fa_sm_gen_StateId_WAIT:
    return "WAIT";
  case fa_sm_gen_StateId::fa_sm_gen_StateId_SNIFF:
    return "SNIFF";
  case fa_sm_gen_StateId::fa_sm_gen_StateId_AUTO:
    return "AUTO";
  case fa_sm_gen_StateId::fa_sm_gen_StateId_ERROR:
    return "ERROR";
  case fa_sm_gen_StateId::fa_sm_gen_StateId_MANUAL:
    return "MANUAL";
  case fa_sm_gen_StateId::fa_sm_gen_StateId_OFF:
    return "OFF";
  case fa_sm_gen_StateId::fa_sm_gen_StateId_ON:
    return "ON";
  case fa_sm_gen_StateId::fa_sm_gen_StateId_ROOT:
    return "ROOT";
  default:
    return "???";
  }
}
#endif
