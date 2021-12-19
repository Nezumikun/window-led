#include <Arduino.h>
#include <FastLED.h>
#include "WindowLed.h"

//#define TEST

#ifdef TEST

#define NUM_LEDS 50
#define DATA_PIN 3
#define TEST_TIME 200
const uint8_t width = 6;
const uint8_t height = 8;
const uint8_t startPoint = START_AT_BOTTOMLEFT;
const uint8_t linesDirection = LINES_HORIZONTAL;
SKIP_INFO skip[1] = {{42, 1}};
const effectInfo effectList[] = {
  { squareSnakeRandom, 10},
  { fill, -5 },
  { towards, -5 },
  { fadeer, 1 },
  { drop, 1 },
  { rainbow, -3 }
};

#else

#define NUM_LEDS 50 * 3
#define TEST_TIME 1000
const uint8_t width = 10;
const uint8_t height = 14;
const uint8_t startPoint = START_AT_BOTTOMLEFT;
const uint8_t linesDirection = LINES_VERTICAL;
SKIP_INFO skip[3] = {{ 0, 5 } , { 5 + 14, 3 }, { 5 + 14 + 3 + 14 * 2, 2 }};
#endif

#define DELAY 50

WindowLed WL(NUM_LEDS, D2, skip, 3, width, height, startPoint, linesDirection);
CRGB leds[NUM_LEDS];
uint8_t ledsHue[NUM_LEDS];
uint8_t hue = 0;
void setRgbXY(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);

unsigned long prev = 0;
void setup() {
  prev = millis();
  Serial.begin(115200);
  Serial.println("Intialized");
  WL.begin();
}

void loop() {
  unsigned long now = millis();
  if (now - prev >= DELAY) {
    // Serial.print (now - prev);
    prev = now;
    WL.update(now);
  }
}