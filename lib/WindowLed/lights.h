#ifndef WINDOW_LED_LIGHTS
#define WINDOW_LED_LIGHTS

#include "effects.h"
#include <FastLED.h>

namespace Nezumikun {
  namespace WindowLed {

    struct EffectsListItem {
      Effect * effect;
      int times;
    };

    struct SkipInfo {
      uint16_t start;
      uint8_t length;
    };

    class Lights {
      protected:
        enum class Mode {
          Init,
          On,
          Off,
          Wait,
          Mode1
        } mode;
        Mode saveMode;
        EffectInit * effectInit;
        EffectRise * effectRise;
        EffectFade * effectFade;
        EffectFill * effectFill;
        Canvas * canvas;
        CRGB * leds;
        uint16_t ledsNumber;
        uint8_t framePerSecond = 25;
        uint16_t timeInMilliseconds = 1000;
        bool firstCall = true;
        SkipInfo * skip = NULL;
        uint8_t skipCount = 0;
        enum class StartAt {
          TopLeft,
          TopRight,
          BottomLeft,
          BottomRight
        } startAt;
        enum class LinesDirection {
          Vertical,
          Hotizontal
        } linesDirection;
        enum class State {
          On,
          Off
        } state;
        uint16_t XY(uint8_t x, uint8_t y);
        void show();
      public:
        Lights(CRGB * leds, uint16_t ledsNumber, uint8_t width, uint8_t height, uint8_t framePerSecond = 25);
        void setSkipInfo(SkipInfo * skip, uint8_t count);
        void loop();
        void on();
        void off();
        bool isOn();
    };

  }
}

#endif