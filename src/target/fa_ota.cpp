// Copyright (c) 2023 by Stefan Schmidt
#include "fa_ota.h"
#include <HTTPClient.h>
#include <Update.h>
#include "fa_log.h"
#include "fa_secrets.h"

fa_ota_t ota;

static HTTPClient client;
static WiFiClient *stream = NULL;

void otaStartIntern(bool simulate)
{
    memset(&ota, 0U, sizeof(ota));
    ota.simulate = simulate;

    client.begin(OTA_HOST);
    ota.http_response = client.GET();
    IMSG(LIOT, "OTA: HTTP Response: ", ota.http_response);

    if (ota.http_response == 200U)
    {
        ota.firmware_size = client.getSize();
        if (!ota.simulate)
        {
            Update.begin(UPDATE_SIZE_UNKNOWN);
        }
        IMSG(LIOT, "OTA: FW Size: ", ota.firmware_size);
        stream = client.getStreamPtr();
        IMSG(LIOT, "OTA: Updating firmware...");
        ota.downloading = true;
    }
    else
    {
        ota.error = true;
        IMSG(LERROR, "OTA: Failed to connect server");
        stream = NULL;
        client.end();
    }
}

void otaUpdate()
{
    if (ota.downloading)
    {
        if (ota.transferred_size < ota.firmware_size)
        {
            if (client.connected())
            {
                uint8_t buf[128] = {0U};
                size_t size = stream->available();
                int n = stream->readBytes(buf, ((size > sizeof(buf)) ? sizeof(buf) : size));
                if (n > 0)
                {
                    if (n <= (ota.firmware_size - ota.transferred_size))
                    {
                        if (!ota.simulate)
                        {
                            Update.write(buf, n);
                        }
                        static uint16_t n_tmp = 0U;
                        ota.transferred_size += n;
                        n_tmp += n;
                        if (n_tmp > (10U * 1024U))
                        {
                            IMSG(LIOT, "OTA: transferred_size", ota.transferred_size);
                            n_tmp = 0U;
                        }
                    }
                    else
                    {
                        IMSG(LERROR, "OTA: Error still data");
                        ota.error = true;
                    }
                }
                if (ota.transferred_size == ota.firmware_size)
                {
                    if (!ota.simulate)
                    {
                        Update.end(true);
                    }
                    IMSG(LIOT, "OTA: Finished!");
                    ota.success = true;
                }
            }
            else
            {
                IMSG(LERROR, "OTA: Connection lost.");
                ota.error = true;
            }
        }

        if (ota.success || ota.error || ota.abort)
        {
            ota.downloading = false;
            stream = NULL;
            client.end();
        }
    }
}

void otaAbort()
{
    ota.abort = true;
}

void otaStart()
{
    IMSG(LIOT, "otaStart()");
    otaStartIntern(false);
}

void otaStartSim()
{
    IMSG(LIOT, "otaStartSim()");
    otaStartIntern(true);
}
