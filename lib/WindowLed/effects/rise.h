#include "with_time.h"

#ifndef WINDOWLED_EFFECT_RISE
#define WINDOWLED_EFFECT_RISE

namespace Nezumikun {
  namespace WindowLed {

    class EffectRise: public EffectWithTime {
      protected:
      public:
        EffectRise(Canvas & canvas, uint8_t framePerSecond, uint16_t timeInMilliseconds = 1000) : EffectWithTime (canvas, framePerSecond, timeInMilliseconds) {
        };
        virtual void loop() override;
    };

  }
}

#endif
