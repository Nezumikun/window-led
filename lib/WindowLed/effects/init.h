#ifndef WINDOWLED_EFFECT_INIT
#define WINDOWLED_EFFECT_INIT

#include "with_time.h"
#include "rise.h"
#include "fade.h"

namespace Nezumikun {
  namespace WindowLed {

    class EffectInit: public EffectWithTime {
      protected:
        struct RandomInfo {
          uint16_t index = 0;
          uint8_t hue;
          uint8_t brightness;
        } randomInfo;
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
        ~EffectInit();
        virtual void reset();
        virtual void loop();
    };

  }
}

#endif
