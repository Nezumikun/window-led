#include "effect.h"

#ifndef WINDOWLED_EFFECT_WITHTIME
#define WINDOWLED_EFFECT_WITHTIME

namespace Nezumikun {
  namespace WindowLed {

    class EffectWithTime: public Effect {
      protected:
        uint8_t frame;
        uint16_t timeInMilliseconds;
      public:
        EffectWithTime(Canvas & canvas, uint8_t framePerSecond, uint16_t timeInMilliseconds = 1000);
        virtual void reset() override;
        virtual void loop() override;
    };

  }
}

#endif
