#include <Arduino.h>
// #include <SPI.h>
// #include <Wire.h>
// #include <Adafruit_GFX.h>
#include <credential.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WebServer.h>
#include <FastLED.h>

void oledCheck();

// IO Pins
#define LED_BOARD_DATA_PIN GPIO_NUM_1


// OLED things
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// LED things
#define NUM_LEDS 768 // 256*3
CRGB leds[NUM_LEDS];

void someTask(void *parameters){
  Serial.println("Some Task");
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void setup()
{
  pinMode(GPIO_NUM_3, INPUT_PULLDOWN);

  Serial.begin(115200);

  // init display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  oledCheck();

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

  xTaskCreate(someTask, "led", 1024, NULL, 0, NULL);

  xTaskCreatePinnedToCore(someTask, "led", 1024 * 5, NULL, 5, NULL, 1);
}

void loop()
{
  Serial.print(digitalRead(GPIO_NUM_3));
  // delay(1000);
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

void oledCheck(){
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2-2; i+=2) {
    display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i,
      display.height()/4, SSD1306_WHITE);
    display.display();
    delay(1);
  }

  delay(1000);
}