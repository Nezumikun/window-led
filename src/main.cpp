#include <Arduino.h>
#include "lights.h"
#include "uptime.h"
#include "LED.h"

#define FRAME_PER_SECOND 50
#define DELAY 1000 / FRAME_PER_SECOND
#define WIFI_CHECK_PERIOD 5000
#define NUM_LEDS 50 * 3

#ifndef DEBUG
#define WIDTH 10
#define HEIGHT 14
#else
#define WIDTH 7
#define HEIGHT 4
#endif
Nezumikun::WindowLed::SkipInfo _skip[3] = {{ 0, 5 } , { 5 + 14, 3 }, { 5 + 14 + 3 + 14 * 2, 2 }};

CRGB leds[NUM_LEDS];
Nezumikun::WindowLed::Lights lights(&leds[0], NUM_LEDS, WIDTH, HEIGHT, FRAME_PER_SECOND);
Nezumikun::Uptime uptime;

#ifdef BOARD_ESP01
Nezumikun::LED led_wifi(2);
#elif BOARD_ESP12F
Nezumikun::LED led_wifi(2);
#else
Nezumikun::LED led_wifi(13);
#endif

unsigned long prevLeds = 0;
unsigned char prevMinutes = -1;

void setup() {
  prevLeds = 500;
  Serial.begin(115200);
  led_wifi.blink(500);
  delay(500);
  Serial.print("Intialized ");
  Serial.print(WIDTH);
  Serial.print("x");
  Serial.print(HEIGHT);
#ifdef BOARD_ARDUINO_NANO
  Serial.print(" BOARD_ARDUINO_NANO");
#endif
#ifdef DEBUG
  Serial.print(" DEBUG");
#endif
  Serial.println();
#ifdef BOARD_ARDUINO_NANO
  FastLED.addLeds<WS2812B, 2, GRB>(leds, NUM_LEDS);
#elif BOARD_ESP01
  FastLED.addLeds<WS2812B, 0, GRB>(leds, NUM_LEDS);
#elif BOARD_ESP12F
  FastLED.addLeds<WS2812B, 4, GRB>(leds, NUM_LEDS);
#else
  FastLED.addLeds<WS2812B, D0, GRB>(leds, NUM_LEDS);
#endif
  //Nezumikun::WindowLed::Settings::debugLevel = Nezumikun::WindowLed::DebugLevel::Debug;
#ifndef DEBUG
  lights.setSkipInfo(_skip, 3);
#else
  lights.setStartAt(Nezumikun::WindowLed::Lights::StartAt::BottomLeft);
  lights.setLinesDirectoin(Nezumikun::WindowLed::Lights::LinesDirection::Horizontal);
  lights.setStartAt(Nezumikun::WindowLed::Lights::StartAt::BottomRight);
  lights.setLinesDirectoin(Nezumikun::WindowLed::Lights::LinesDirection::Vertical);
#endif
  uptime.reset();
}

void loop() {
  unsigned long now = millis();
  if (now - prevLeds >= DELAY) {
    prevLeds = prevLeds + DELAY;
    lights.loop();
  }
  uptime.loop();
#ifdef DEBUG
  //delay(50);
#endif
  if (prevMinutes != uptime.info.minutes) {
    Serial.print(F("Uptime: "));
    Serial.print(uptime.toString());
    Serial.println();
    prevMinutes = uptime.info.minutes;
  }
  led_wifi.touch();
}