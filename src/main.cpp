#include "main.h"
#include <Arduino.h>
#include <Wire.h>
// the oled display
#include <Adafruit_SSD1306.h>
// the wifi
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// the main led
#include <FastLED.h>

#include "credential.h"
#include "config.h"
#include "web.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

CRGB leds[NUM_LEDS];

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");


void someTask(void *parameters)
{
  Serial.println("Some Task");
  delay(1000);
}

void setup()
{
  pinMode(GPIO_NUM_3, INPUT_PULLDOWN);
  Wire.setPins(GPIO_NUM_4, GPIO_NUM_5);
  Wire.begin();
  Serial.begin(115200);
  
  // init display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  oledCheck();
  // delay(1000);

  // Init LEDs
  FastLED.addLeds<WS2812B, LED_BOARD_DATA_PIN, GRB>(leds, NUM_LEDS);

  // connect wifi
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.setCursor(0, 10);
  display.println("Connecting to WiFi");
  display.display();

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    display.print('.');
    delay(1000);
  }

  auto lanIP = WiFi.localIP();
  Serial.println(lanIP);

  display.clearDisplay();
  display.setCursor(0, 10);
  display.println("This device IP:");
  display.println(lanIP.toString());
  display.display();



  initWebSocket(&server, &ws);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", "<p>Hello</p>");
  });

  // Start web server
  server.begin();
  display.println("Server started.");
  display.display();

  // xTaskCreate(ledScroller, "led_scroll", 1024, NULL, 0, NULL);
}

void loop()
{
  printf(".");
  vTaskDelay(1000 / portTICK_RATE_MS);

}

void ledScroller(void *parameters)
{
  while (1)
  {
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = CRGB::Green;
      FastLED.show();
      delay(2);
    }
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = CRGB::Red;
      FastLED.show();
      delay(2);
    }
  }
}

void oledCheck()
{
  Serial.println("Checking OLED");
  display.clearDisplay();

  for (int16_t i = 0; i < display.height() / 2 - 2; i += 2)
  {
    display.drawRect(i, i, display.width() - 2 * i, display.height() - 2 * i,
                           SSD1306_WHITE);
    display.display();
    delay(1);
  }
  display.drawRect(0, 0, display.width() - 1, display.height() - 1, SSD1306_WHITE);
  display.display();
  delay(500);
  display.fillRect(0, 0, display.width() - 1, display.height() - 1, SSD1306_WHITE);
  display.display();
  delay(500);
  display.clearDisplay();
}