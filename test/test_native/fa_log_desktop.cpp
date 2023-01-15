// Copyright (c) 2023 by Stefan Schmidt
#include "fa_log.h"
#include <stdio.h>
#include "fa_settings.h"

void loggingSetup()
{
}

void IMSG(uint8_t mask, const char *txt)
{
    CHECK_LOG_MASK(mask);
    printf("%s\n", txt);
}

void IMSG(uint8_t mask, float val)
{
    CHECK_LOG_MASK(mask);
    printf("%f\n", val);
}

void IMSG(uint8_t mask, const char *txt, float val)
{
    CHECK_LOG_MASK(mask);
    printf("%s: %f\n", txt, val);
}

void IMSGHEX(uint8_t mask, uint8_t val)
{
    CHECK_LOG_MASK(mask);
}

void IMSG(uint8_t mask, const char *txt, const char *val)
{
    CHECK_LOG_MASK(mask);
    printf("%s: %s\n", txt, val);
}

void IMSG(uint8_t mask, const char *txt1, const char *txt2, const char *txt3, const char *txt4)
{
    CHECK_LOG_MASK(mask);
    printf("%s %s %s %s\n", txt1, txt2, txt3, txt4);
}