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
      public:
        enum class StartAt {
          TopLeft,
          TopRight,
          BottomLeft,
          BottomRight
        };
        enum class LinesDirection {
          Vertical,
          Horizontal
        };
      protected:
        enum class Mode {
          Init,
          On,
          Off,
          Wait,
          Demo
        } mode;
        Mode saveMode;
        EffectInit * effectInit;
        EffectRise * effectRise;
        EffectFade * effectFade;
        Effect ** effects;
        uint8_t effectCount;
        uint8_t currentEffect;
        Canvas * canvas;
        CRGB * leds;
        uint16_t ledsNumber;
        uint8_t framePerSecond = 25;
        uint16_t timeInMilliseconds = 1000;
        bool firstCall = true;
        SkipInfo * skip = NULL;
        uint8_t skipCount = 0;
        StartAt startAt;
        LinesDirection linesDirection;
        enum class State {
          On,
          Off
        } state;
        uint16_t XY(uint8_t x, uint8_t y);
        void show();
      public:
        Lights(CRGB * leds, uint16_t ledsNumber, uint8_t width, uint8_t height, uint8_t framePerSecond = 25);
        void setSkipInfo(SkipInfo * skip, uint8_t count);
        void setStartAt(StartAt startAt);
        void setLinesDirectoin(LinesDirection linesDirection);
        void loop();
        void on();
        void off();
        bool isOn();
    };

  }
}

#endif