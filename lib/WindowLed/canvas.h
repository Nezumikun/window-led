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
        CRGB * leds;
      public:
        Canvas(uint8_t width, uint8_t height);
        uint8_t getWidth();
        uint8_t getHeight();
        CRGB * getLeds();
    };

  }
}

#endif
