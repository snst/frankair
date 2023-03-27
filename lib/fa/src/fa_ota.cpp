// Copyright (c) 2023 by Stefan Schmidt
#include "fa_ota.h"

fa_ota_t ota = {0};

bool isOtaActive()
{
    return ota.downloading;
}

