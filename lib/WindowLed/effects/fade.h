#include "with_time.h"

#ifndef WINDOWLED_EFFECT_FADE
#define WINDOWLED_EFFECT_FADE

namespace Nezumikun {
  namespace WindowLed {

    class EffectFade: public EffectWithTime {
      protected:
      public:
        EffectFade(Canvas & canvas, uint8_t framePerSecond, uint16_t timeInMilliseconds = 1000) : EffectWithTime (canvas, framePerSecond, timeInMilliseconds) {
        };
        virtual void loop();
    };

  }
}

#endif
