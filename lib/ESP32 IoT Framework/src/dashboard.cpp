#include "dashboard.h"
#include "webServer.h"

void dashboard::begin(int sampleTimeMs)
{
    GUI.ws.onEvent(onWsEvent);
    loopRate = sampleTimeMs;
}

void dashboard::loop()
{
    if (loopPrevious == 0 || (millis() - loopPrevious > loopRate))
    {
        loopPrevious = millis();

        send();
    }
}

void dashboard::send()
{
    //send data, first 32bit timestamp and then the binary data structure
    uint8_t buffer[sizeof(data) + 8 + 8 + 8 + sizeof(historicdata)];

    unsigned long now = millis();
    memcpy(buffer, reinterpret_cast<uint8_t *>(&now), 8);
    unsigned long dashdatalength = sizeof(data);
    memcpy(buffer + 8, reinterpret_cast<uint8_t *>(&dashdatalength), 8);

    unsigned long historicdatalength = cihistoricdatalength;
    memcpy(buffer + 8 + 8, reinterpret_cast<uint8_t *>(&historicdatalength), 8);

    memcpy(buffer + 8 + 8 + 8, reinterpret_cast<uint8_t *>(&data), sizeof(data));

    memcpy(buffer + 8 + 8 + 8 + sizeof(data), reinterpret_cast<uint8_t *>(&historicdata), sizeof(historicdata));

    // Serial.printf("Dashsize:%d historicdatalegth:%d historicsize:%d buffersize:%d \n",dashdatalength,historicdatalength,sizeof(historicdata),sizeof(buffer));
    GUI.ws.binaryAll(buffer, sizeof(buffer));
}

void dashboard::onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *dataIn, size_t len)
{
    /* initialize new client */
    if (type == WS_EVT_CONNECT)
    {
        Serial.println("New WS client");
    }
    else if (type == WS_EVT_DISCONNECT)
    {
        Serial.println("Lost WS client");
    }
}

dashboard dash;