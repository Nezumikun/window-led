#ifndef WINDOWLED_EFFECT_INIT
#define WINDOWLED_EFFECT_INIT

#include "with_time.h"
#include "rise.h"
#include "fade.h"

namespace Nezumikun {
  namespace WindowLed {

    class EffectInit: public EffectWithTime {
      protected:
        uint8_t hue;
        enum class Mode {
          Red, Green, Blue, White, Random, End
        } mode;
        enum class SubMode {
          Rise, Fade, Draw
        } subMode;
        EffectRise *effectRise;
        EffectFade *effectFade;
      public:
        EffectInit(Canvas & canvas, uint8_t framePerSecond, uint16_t timeInMilliseconds = 1000);
        virtual void loop();
    };

  }
}

#endif
