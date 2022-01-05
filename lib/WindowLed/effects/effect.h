#include <FastLED.h>
#include "../canvas.h"

#ifndef WINDOWLED_EFFECT
#define WINDOWLED_EFFECT

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
        virtual ~Effect();
        bool endOfEffect();
        virtual void loop() = 0;
    };

  }
}

#endif
