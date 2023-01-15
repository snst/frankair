// Copyright (c) 2023 by Stefan Schmidt
#include "fa_ota.h"
#include <HTTPClient.h>
#include <Update.h>
#include "fa_log.h"
#include "fa_secrets.h"

fa_ota_t fa_ota;

static HTTPClient client;
static WiFiClient *stream = NULL;

void otaStartIntern(bool simulate)
{
    memset(&fa_ota, 0U, sizeof(fa_ota));
    fa_ota.simulate = simulate;

    client.begin(OTA_HOST);
    fa_ota.http_response = client.GET();
    IMSG(LM_COMMON, "OTA: HTTP Response: ", fa_ota.http_response);

    if (fa_ota.http_response == 200)
    {
        fa_ota.firmware_size = client.getSize();
        if (!fa_ota.simulate)
        {
            Update.begin(UPDATE_SIZE_UNKNOWN);
        }
        IMSG(LM_COMMON, "OTA: FW Size: ", fa_ota.firmware_size);
        stream = client.getStreamPtr();
        IMSG(LM_COMMON, "OTA: Updating firmware...");
        fa_ota.downloading = true;
    }
    else
    {
        fa_ota.error = true;
        IMSG(LM_COMMON, "OTA: Failed to connect server");
        stream = NULL;
        client.end();
    }
}

void otaUpdate()
{
    if (fa_ota.downloading)
    {
        if (fa_ota.transferred_size < fa_ota.firmware_size)
        {
            if (client.connected())
            {
                uint8_t buf[128] = {0};
                size_t size = stream->available();
                int n = stream->readBytes(buf, ((size > sizeof(buf)) ? sizeof(buf) : size));
                if (n > 0)
                {
                    if (n <= (fa_ota.firmware_size - fa_ota.transferred_size))
                    {
                        if (!fa_ota.simulate)
                        {
                            Update.write(buf, n);
                        }
                        static uint16_t n_tmp = 0U;
                        fa_ota.transferred_size += n;
                        n_tmp += n;
                        if (n_tmp > (10U * 1024U))
                        {
                            IMSG(LM_COMMON, "OTA: transferred_size", fa_ota.transferred_size);
                            n_tmp = 0U;
                        }
                    }
                    else
                    {
                        IMSG(LM_COMMON, "OTA: Error still data");
                        fa_ota.error = true;
                    }
                }
                if (fa_ota.transferred_size == fa_ota.firmware_size)
                {
                    if (!fa_ota.simulate)
                    {
                        Update.end(true);
                    }
                    IMSG(LM_COMMON, "OTA: Finished!");
                    fa_ota.success = true;
                }
            }
            else
            {
                IMSG(LM_COMMON, "OTA: Connection lost.");
                fa_ota.error = true;
            }
        }

        if (fa_ota.success || fa_ota.error || fa_ota.abort)
        {
            fa_ota.downloading = false;
            stream = NULL;
            client.end();
        }
    }
}

void otaAbort()
{
    fa_ota.abort = true;
}

void otaStart()
{
    IMSG(LM_COMMON, "otaStart()");
    otaStartIntern(false);
}

void otaStartSim()
{
    IMSG(LM_COMMON, "otaStartSim()");
    otaStartIntern(true);
}
