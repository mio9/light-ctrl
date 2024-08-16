#include "web.h"
#include <FastLED.h>

void initWebSocket(AsyncWebServer *server, AsyncWebSocket *ws)
{
    ws->onEvent(onWebEvent);
    server->addHandler(ws);
}

void onWebEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
                void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        break;
    case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;
    case WS_EVT_DATA:
        handleWebSocketMessage(NULL, arg, data, len);
        break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
        break;
    }
}

void handleWebSocketMessage(AsyncWebSocket *ws, void *arg, uint8_t *data, size_t len)
{
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
    {
        for (int i = 0; i < len; i++)
        {
            Serial.print((int)data[i]);
        }
        Serial.print("\n");
        // Serial.println(data);
        CRGB *leds = FastLED.leds();



        // if (strcmp((char*)data, "toggle") == 0) {
        //   Serial.println("Toggling LED");
        //   ws->textAll(String("toggled"));
        // }
    }
}