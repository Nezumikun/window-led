#include "paletted_noise.h"
#include "../settings.h"
#include "gyver_palettes.h"
namespace Nezumikun {
  namespace WindowLed {

    EffectPalettedPerlinNoise::EffectPalettedPerlinNoise(Canvas & canvas, uint8_t framePerSecond, uint32_t timeInMilliseconds) : EffectWithTime (canvas, framePerSecond, timeInMilliseconds) {
      this->firstCall = true;
      this->x = random16();
      this->y = random16();
      this->z = random16();
      this->hue = random8();
      this->hueCounter = framePerSecond;
    };

    void EffectPalettedPerlinNoise::reset() {
      uint8_t count = sizeof(paletteArr) / sizeof(CRGBPalette16);
      Serial.print(F("Palletes count = "));
      Serial.print(count);
      Serial.println();
      this->currentPalette = random8(count);
      Serial.print(F("Selected palette = "));
      Serial.print(this->currentPalette);
      Serial.println();
      this->canvas->setRotate(Canvas::Rotate::None);
      this->canvas->setBrightness(255);
      this->frame = 0;
      Effect::reset();
    }

    void EffectPalettedPerlinNoise::effectFinished() {
      this->_endOfEffect = true;
    };

    const __FlashStringHelper * EffectPalettedPerlinNoise::getName() {
      return F("Paletted Perlin Noise");
    }

    void EffectPalettedPerlinNoise::loop() {
      this->_endOfEffect = false;
      Effect::loop();
      this->frame++;
      uint8_t width = this->canvas->getWidth(false);
      CRGB * leds = this->canvas->getLeds();
      for (uint8_t i = 0; i < this->canvas->getHeight(false); i++) {
        for (uint8_t j = 0; j < width; j++) {
          CRGB color = ColorFromPalette(paletteArr[this->currentPalette], inoise8(i * scale, j * scale, this->z / 10));
          leds[this->canvas->XY(j, i)] = color;
        }
      }
      this->z += this->speed;
      if (((uint32_t)this->frame * (1000 / this->framePerSecond)) >= this->timeInMilliseconds) {
        this->effectFinished();
      }
   }

  }
}