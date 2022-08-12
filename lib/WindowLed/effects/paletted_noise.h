#ifndef WINDOWLED_EFFECT_PALETTED_PERLINNOISE
#define WINDOWLED_EFFECT_PALETTED_PERLINNOISE

#include "with_time.h"
#include "rise.h"
#include "fade.h"

namespace Nezumikun {
  namespace WindowLed {

    class EffectPalettedPerlinNoise: public EffectWithTime {
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
        const char * name = "Paletted Perlin Noise";
      public:
        EffectPalettedPerlinNoise(Canvas & canvas, uint8_t framePerSecond, uint32_t timeInMilliseconds = 1000);
        ~EffectPalettedPerlinNoise();
        virtual void reset() override;
        virtual void loop() override;
        virtual const char * getName() override;
    };

  }
}

#endif
