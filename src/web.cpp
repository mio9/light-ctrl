#include "web.h"
#include "config.h"
#include <FastLED.h>
#include <base64.hpp>

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
        uint8_t ledData[3];
        unsigned char base64[3] = {0};
        for (int i = 0; i < len; i++)
        {
           base64[i] = data[i];
        }
        decode_base64(base64, ledData);
        Serial.print("\n");
        

        CRGB *leds = FastLED.leds();
        CRGB color = CRGB(ledData[0], ledData[1], ledData[2]);

        // set color to all led
        for (int i = 0; i < NUM_LEDS; i++){
            leds[i] = color;
        }
        FastLED.show();
        
        

        // if (strcmp((char*)data, "toggle") == 0) {
        //   Serial.println("Toggling LED");
        //   ws->textAll(String("toggled"));
        // }
    }
}