#ifndef WINDOWLED_EFFECT_FIRE
#define WINDOWLED_EFFECT_FIRE

#include "with_time.h"


namespace Nezumikun {
  namespace WindowLed {

    class EffectFire: public EffectWithTime {
      protected:
        CRGBPalette16 myPal;
        CRGB * data;
        uint16_t a = 0;
      public:
        EffectFire(Canvas & canvas, uint8_t framePerSecond, uint32_t timeInMilliseconds = 10000);
        ~EffectFire();
        virtual void reset() override;
        virtual void loop() override;
        virtual const __FlashStringHelper * getName() override;
    };

  }
}

#endif
