#ifndef WINDOWLED_EFFECT_INIT
#define WINDOWLED_EFFECT_INIT

#include "with_time.h"
#include "blink.h"
#include "fade.h"

namespace Nezumikun {
  namespace WindowLed {

    class EffectInit: public EffectWithTime {
      protected:
        enum class Mode {
          Red, Green, Blue, White, Random, End
        } mode;
        enum class SubMode {
          Draw, Blink
        } subMode;
        EffectFade *effectFade;
        EffectBlink *effectBlink;
      public:
        EffectInit(Canvas & canvas, uint8_t framePerSecond, uint32_t timeInMilliseconds = 1000);
        ~EffectInit();
        virtual void reset() override;
        virtual void loop() override;
    };

  }
}

#endif
