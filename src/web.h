#include <ESPAsyncWebServer.h>

void initWebSocket(AsyncWebServer *server, AsyncWebSocket *ws);
void onWebEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len);

void handleWebSocketMessage(AsyncWebSocket *ws, void *arg, uint8_t *data, size_t len);