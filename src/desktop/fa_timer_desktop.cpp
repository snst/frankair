// Copyright (c) 2023 by Stefan Schmidt
#include "fa_timer.h"
#include <math.h>
#include <stdio.h>
#include <sys/time.h>
#include <chrono>
#include <thread>
/*
uint32_t FAEventManager::now()
{
    struct timeval te;
    gettimeofday(&te, NULL);
    long long milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000;
    return (uint32_t)milliseconds;
}

void FAEventManager::sleep(uint32_t ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
*/