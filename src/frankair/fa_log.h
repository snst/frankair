// Copyright (c) 2022 by Stefan Schmidt
#ifndef FA_LOG_H
#define FA_LOG_H

void logging_setup();
void IMSG(float val);
void IMSG(const char *txt);
void IMSG(const char *txt, float val);
void IMSGHEX(uint8_t val);

#endif // FA_LOG_H
