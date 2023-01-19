// Copyright (c) 2023 by Stefan Schmidt
#ifndef FA_THING_PRIV_H
#define FA_THING_PRIV_H

#include "fa_common.h"
#include <ThingerESP32.h>

#define WIFI_MAX_CNT 2U
#define WIFI_TIMEOUT 10000U

#define ADD_CMD(name, action) \
  thing[name] = []()          \
  {                           \
    IMSG(LM_THING, name);     \
    action();                 \
  };

class FAThingerESP32 : public ThingerESP32
{
public:
  FAThingerESP32(const char *user,              //
                 const char *device,            //
                 const char *device_credential) //
      : ThingerESP32(user, device, device_credential),
        wifi_cnt(0),
        wifi_current(0),
        state(0)
  {
  }

  void add_wifi2(const char *wifi_ssd, const char *wifi_password)
  {
    if (wifi_cnt < WIFI_MAX_CNT)
    {
      wifi_ssid_array[wifi_cnt] = wifi_ssd;
      wifi_password_array[wifi_cnt] = wifi_password;
      wifi_cnt++;
    }
  }

  void next_wifi()
  {
    wifi_current = (wifi_current + 1U) % wifi_cnt;
  }

  const char *get_wifi_ssid()
  {
    return wifi_ssid_array[wifi_current];
  }

  const char *get_wifi_password()
  {
    return wifi_password_array[wifi_current];
  }

  virtual bool connect_network()
  {
    /*
    if (WiFi.status() == WL_CONNECTED) {
      return true;
    }*/

    switch (state)
    {
    case 0:
      if (get_wifi_ssid() != nullptr)
      {
        IMSG(LM_THING, "Connecting to network: ", get_wifi_ssid());
        WiFi.begin((char *)get_wifi_ssid(), (char *)get_wifi_password());
        wifi_timeout = millis();
        state++;
      }
      else
      {
        IMSG(LM_THING, "Missing network config\n");
      }
      break;
    case 1:
      if (WiFi.status() == WL_CONNECTED)
      {
        IMSG(LM_THING, "WL_CONNECTED");
        wifi_timeout = millis();
        state++;
      }
      else
      {
        if (millis() - wifi_timeout > WIFI_TIMEOUT)
        {
          next_wifi();
          IMSG(LM_THING, "WIFI Timeout 1, next ssid: ", get_wifi_ssid());
          state = 0U;
        }
      }
      break;
    case 2:
      if ((WiFi.localIP() != (IPAddress)INADDR_NONE))
      {
        IMSG(LM_THING, "Got IP Address: ");
        wifi_timeout = millis();
        state++;
        return true;
      }
      else
      {
        if (millis() - wifi_timeout > WIFI_TIMEOUT)
        {
          IMSG(LM_THING, "WIFI Timeout 2");
          WiFi.disconnect();
          state = 0U;
        }
      }
      break;
    case 3:
      if (millis() - wifi_timeout > WIFI_TIMEOUT)
      {
        IMSG(LM_THING, "WIFI Timeout 3");
        WiFi.disconnect();
        state = 0U;
        return false;
      }
      return true;
    }
    return false;
  }

protected:
  uint8_t wifi_cnt;
  const char *wifi_ssid_array[WIFI_MAX_CNT];
  const char *wifi_password_array[WIFI_MAX_CNT];
  uint8_t wifi_current;
  uint8_t state;
  unsigned long wifi_timeout;
};

#endif // FA_THING_PRIV_H
