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
      public:
        Effect(Canvas & canvas);
        bool endOfEffect();
        virtual void nextTick(bool firstCall) = 0;
    };

    class EffectInit: public Effect {
      protected:
        unsigned char step;
        unsigned char value;
      public:
        EffectInit(Canvas & canvas) : Effect(canvas) {
        };
        virtual void nextTick(bool firstCall);
    };

  }
}

#endif