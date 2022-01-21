#ifndef WINDOWLED_HELPERS_DOT
#define WINDOWLED_HELPERS_DOT

#include <FastLED.h>

namespace Nezumikun {
  namespace WindowLed {
    namespace Helpers {

      class Dot {
        protected:
          CRGB originalColor;
          CRGB color;
          uint8_t framePerSecond;
          uint16_t timeInMilliseconds;
          bool firstCall = true;
          bool lastCall = false;
          uint8_t frame = 0;
          uint8_t frameLimit = 0;
          uint8_t brightness = 0;
          bool isFirstCall();
          bool isLastCall();
        public:
          Dot(uint8_t framePerSecond, uint16_t timeInMilliseconds = 1000);
          void setColor(CRGB color);
          void setColor(uint8_t hue);
          CRGB getColor();
          uint8_t getBrightness();
          CRGB rise();
          CRGB fade();
          bool endOfEffect();
      };

    }
  }
}

#endif
