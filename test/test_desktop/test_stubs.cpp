// Copyright (c) 2023 by Stefan Schmidt
#include <stdint.h>

void IMSG(uint8_t mask, float val) {}
void IMSG(uint8_t mask, const char *txt) {}
void IMSG(uint8_t mask, const char *txt1, const char *txt2, const char *txt3, const char *txt4) {}
void IMSG(uint8_t mask, const char *txt, float val) {}
void IMSG(uint8_t mask, const char *txt, const char *val) {}
void IMSGHEX(uint8_t mask, uint8_t val) {}
bool ledEnable(bool enable) { return false; }
void settingsDefaultPlatform() {}
void thingSetupPlatform() {}
void thingSendStreamState() {}
uint32_t getMillis() { return 0U; }
void rebootTarget() {}
void flapSetOpenPWM(unsigned char) {}
void fanSetPWM(unsigned char, unsigned char, unsigned char) {}
void thingCallEndpoint(char const*) {}
void sensorsRead() {}
void sensorsSetupPlatform() {}
