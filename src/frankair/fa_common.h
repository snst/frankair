// Copyright (c) 2022 by Stefan Schmidt
#ifndef FA_COMMON_H
#define FA_COMMON_H

bool interval(uint32_t &last, uint32_t delay);
void interval_update();
uint32_t now();

template <typename T>
bool update_if_changed(T &dest, T src, const char *txt);

#endif // FA_COMMON_H
