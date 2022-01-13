#include <FastLED.h>

#ifndef WINDOWLED_CANVAS
#define WINDOWLED_CANVAS

namespace Nezumikun {
  namespace WindowLed {

    class Canvas {
      public:
        enum class Rotate {
          None,
          Angle90,
          Angle180,
          Angle270
        };
      private:
        uint8_t width;
        uint8_t height;
        uint16_t ledsNumber;
        CRGB * leds;
        uint8_t brightness = 100;
        Rotate anngle = Rotate::None;
      public:
        Canvas(uint8_t width, uint8_t height);
        uint8_t getWidth(bool useRotateSettings = true);
        uint8_t getHeight(bool useRotateSettings = true);
        uint8_t getBrightness();
        uint16_t getLedsNumber();
        void setBrightness(uint8_t brightness);
        CRGB * getLeds();
        uint16_t XY(uint8_t x, uint8_t y, bool useRotateSettings = true);
        void fill(const struct CRGB & color);
        void setRotate(Rotate angle);
        void setRotateRandom();
    };

  }
}

#endif
