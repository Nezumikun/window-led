#ifndef WINDOWLED_EFFECT_PERLINNOISE
#define WINDOWLED_EFFECT_PERLINNOISE

#include "with_time.h"
#include "rise.h"
#include "fade.h"

namespace Nezumikun {
  namespace WindowLed {

    class EffectPerlinNoise: public EffectWithTime {
      protected:
        uint16_t x;
        uint16_t y;
        uint16_t z;
        uint8_t hue;
        uint8_t hueCounter;

        uint16_t speed = 2; // a nice starting speed, mixes well with a scale of 100
        // uint16_t speed = 33;
        // uint16_t speed = 100; // wicked fast!

        // uint16_t scale = 1; // mostly just solid colors
        // uint16_t scale = 4011; // very zoomed out and shimmery
        uint16_t scale = 50;
        uint8_t * data;
        virtual void effectFinished() override;
      public:
        EffectPerlinNoise(Canvas & canvas, uint8_t framePerSecond, uint32_t timeInMilliseconds = 1000);
        ~EffectPerlinNoise();
        virtual void reset() override;
        virtual void loop() override;
        virtual const __FlashStringHelper * getName() override;
    };

  }
}

#endif
