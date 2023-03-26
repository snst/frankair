// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_OTA_H
#define FA_OTA_H

#include <stdlib.h>
#include <stdint.h>

struct fa_ota_t
{
  uint32_t firmware_size;
  uint32_t transferred_size;
  uint32_t checksum;
  int http_response;
  bool downloading;
  bool download_finished;
  bool abort;
  bool error;
  bool success;
  bool simulate;
};

extern fa_ota_t ota;

void otaStartSim();
void otaStart();
void otaAbort();
void otaUpdate();

#endif // FA_OTA_H
