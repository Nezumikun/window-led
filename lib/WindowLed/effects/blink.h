#ifndef WINDOWLED_EFFECT_BLINK
#define WINDOWLED_EFFECT_BLINK

#include "with_time.h"
#include "rise.h"
#include "fade.h"

namespace Nezumikun {
  namespace WindowLed {

    class EffectBlink: public EffectWithTime {
      protected:
        enum class Mode {
          Rise, Fade
        } mode;
        EffectRise *effectRise;
        EffectFade *effectFade;
        CRGB color;
      public:
        EffectBlink(Canvas & canvas, uint8_t framePerSecond, uint32_t timeInMilliseconds = 1000);
        ~EffectBlink();
        virtual void reset() override;
        virtual void loop() override;
        void setColor(CRGB color);
        void setColor(uint8_t hue);
        void setRandomColor();
    };

  }
}

#endif
