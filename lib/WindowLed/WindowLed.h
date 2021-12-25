#ifndef WINDOW_LED
#define WINDOW_LED
#include <Arduino.h>
#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

#define START_AT_TOPLEFT 0
#define START_AT_TOPRIGHT 1
#define START_AT_BOTTOMLEFT 2
#define START_AT_BOTTOMRIGHT 3

#define LINES_VERTICAL 0
#define LINES_HORIZONTAL 1

#define WINDOWLED_TEST_TIME 20

#define WINDOWLED_INIT 0xFF
#define WINDOWLED_RAINBOW 0x00
#define WINDOWLED_DROP 0x01
#define WINDOWLED_FADE 0x02
#define WINDOWLED_SQARESNAKE 0x03
#define WINDOWLED_TOWARDS 0x04
#define WINDOWLED_FILL 0x05

namespace WL {
  struct SKIP_INFO {
    uint8_t start;
    uint8_t length;
  };

  struct effectInfo {
    uint8_t funcNumber;
    int times;
  };

  struct FADE_PARAMS {
    uint8_t value;
    uint8_t hue;
    bool fade;
    bool skip;
  };

  struct DROP_PARAMS {
    uint8_t inc;
    uint8_t speed;
    uint8_t y;
    uint8_t amount;
    uint8_t height;
    uint8_t hue;
  };

  class WindowLed {
    private:
      bool isOn;
      bool effectInit(bool firstStart);
      bool effectRainbow(bool firstStart);
      bool effectDrop(bool firstStart);
      bool effectFade(bool firstStart);
      bool effectSquareSnake(bool firstStart, bool changeColor);
      bool effectSquareSnake(bool firstStart);
      bool effectTowards(bool firstStart);
      bool effectFill(bool firstStart);
      uint8_t LedsNumber;
      uint8_t width;
      uint8_t height;
      uint8_t startPoint;
      uint8_t linesDirection;
      uint8_t controlPin;
      SKIP_INFO* skip;
      uint8_t skipCount;
      effectInfo* effectList;
      uint8_t effectCount;
      CRGB* leds;
      uint8_t* ledsHue;
      int XY(uint8_t x, uint8_t y);
      void setHueXY(uint8_t x, uint8_t y, uint8_t hue);
      void setHsvXY(uint8_t x, uint8_t y, uint8_t hue, uint8_t sat, uint8_t value);
      void setRgbXY(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);
      void fillRgb(uint8_t r, uint8_t g, uint8_t b);
      void copyXY(uint8_t fromX, uint8_t fromY, uint8_t toX, uint8_t toY);
      uint8_t getEffectTimes(unsigned char index);
      uint8_t currentEffect;
      uint8_t currentTime;
      void showCurrentSequence(unsigned long now);
    public:
      WindowLed(uint8_t LedsNumber, uint8_t controlPin, SKIP_INFO* skip, uint8_t skipCount, uint8_t width, uint8_t height, uint8_t startPoint, uint8_t linesDirection);
      void begin();
      void update(unsigned long now);

      void on();
      void off();
      bool getState();
  };

}

#endif