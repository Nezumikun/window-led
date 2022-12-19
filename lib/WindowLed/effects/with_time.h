#include "effect.h"

#ifndef WINDOWLED_EFFECT_WITHTIME
#define WINDOWLED_EFFECT_WITHTIME

namespace Nezumikun {
  namespace WindowLed {

    class EffectWithTime: public Effect {
      protected:
        uint32_t frame;
        uint32_t timeInMilliseconds;
      public:
        EffectWithTime(Canvas & canvas, uint8_t framePerSecond, uint32_t timeInMilliseconds = 1000);
        virtual void reset() override;
        virtual void loop() override;
    };

  }
}

#endif
