#ifndef WINDOW_LED_EFFECTS
#define WINDOW_LED_EFFECTS
#include <FastLED.h>
#include "canvas.h"

namespace Nezumikun {
  namespace WindowLed {

    class Effect {
      protected:
        Canvas * canvas;
        bool _endOfEffect = false;
        bool firstCall = true;
        uint8_t framePerSecond;
        void effectFinished();
      public:
        Effect(Canvas & canvas, uint8_t framePerSecond);
        bool endOfEffect();
        virtual void loop() = 0;
    };

    class EffectWithTime: public Effect {
      protected:
        uint8_t frame;
        uint16_t timeInMilliseconds;
      public:
        EffectWithTime(Canvas & canvas, uint8_t framePerSecond, uint16_t timeInMilliseconds = 1000);
        virtual void loop() = 0;
    };

    class EffectRise: public EffectWithTime {
      protected:
      public:
        EffectRise(Canvas & canvas, uint8_t framePerSecond, uint16_t timeInMilliseconds = 1000) : EffectWithTime (canvas, framePerSecond, timeInMilliseconds) {
          Serial.println(__FUNCTION__);
        };
        virtual void loop();
    };

    class EffectInit: public Effect {
      protected:
        unsigned char step;
        unsigned char value;
      public:
        EffectInit(Canvas & canvas, uint8_t framePerSecond) : Effect(canvas, framePerSecond) {
        };
        virtual void loop();
    };

  }
}

#endif