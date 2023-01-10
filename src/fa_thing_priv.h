// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_THING_PRIV_H
#define FA_THING_PRIV_H

#include "fa_common.h"
#include <ThingerESP32.h>

#define ADD_CMD(name, action) \
  thing[name] = []()          \
  {                           \
    IMSG(LM_THING, name);     \
    action();                 \
  };

class FAThingerESP32 : public ThingerESP32
{
public:
  FAThingerESP32(const char *user, const char *device, const char *device_credential) : ThingerESP32(user, device, device_credential)
  {
  }

protected:
  virtual bool connect_network()
  {
    unsigned long wifi_timeout = millis();

    if (wifi_ssid_ != nullptr)
    {
      IMSG(LM_THING, "Connecting to network ", wifi_ssid_);
      WiFi.begin((char *)wifi_ssid_, (char *)wifi_password_);
    }
// espressif esp8266 and esp32 can connect to last network just with begin() (used with WiFiManager)
#if defined(ESP8266) || defined(ESP32)
    else
    {
      IMSG(LM_THING, "Connecting to stored network\n");
      WiFi.begin();
    }
#else
    else
    {
      IMSG("Cannot connect to WiFi. SSID not set!\n");
      return false;
    }
#endif

    while (WiFi.status() != WL_CONNECTED)
    {
      if (millis() - wifi_timeout > 30000)
        return false;
#ifdef ESP8266
      yield();
#endif
    }

    IMSG(LM_THING, "Connected to WiFi!");
    wifi_timeout = millis();
    IMSG(LM_THING, "Getting IP Address...");
    while (WiFi.localIP() == (IPAddress)INADDR_NONE)
    {
      if (millis() - wifi_timeout > 30000)
        return false;
#ifdef ESP8266
      yield();
#endif
    }
    IMSG(LM_THING, "Got IP Address: ");
    return true;
  }
};

#endif // FA_THING_PRIV_H
