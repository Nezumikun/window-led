#include <Arduino.h>
#include <FastLED.h>

#ifndef WINDOWLED_CANVAS
#define WINDOWLED_CANVAS

namespace Nezumikun {
  namespace WindowLed {

    class Canvas {
      private:
        uint8_t width;
        uint8_t height;
        uint16_t ledsNumber;
        CRGB * leds;
        uint8_t brightness = 100;
      public:
        Canvas(uint8_t width, uint8_t height);
        uint8_t getWidth();
        uint8_t getHeight();
        uint8_t getBrightness();
        void setBrightness(uint8_t brightness);
        CRGB * getLeds();
        void fill(const struct CRGB & color);
    };

  }
}

#endif
