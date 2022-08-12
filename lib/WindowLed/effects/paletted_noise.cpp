#include "paletted_noise.h"
#include "../settings.h"
namespace Nezumikun {
  namespace WindowLed {

    EffectPalettedPerlinNoise::EffectPalettedPerlinNoise(Canvas & canvas, uint8_t framePerSecond, uint32_t timeInMilliseconds) : EffectWithTime (canvas, framePerSecond, timeInMilliseconds) {
      this->firstCall = true;
      this->x = random16();
      this->y = random16();
      this->z = random16();
      this->hue = random8();
      this->hueCounter = framePerSecond;
      this->data = (uint8_t *) malloc(canvas.getLedsNumber() * sizeof(uint8_t));
    };

    EffectPalettedPerlinNoise::~EffectPalettedPerlinNoise() {
      free(this->data);
    }

    void EffectPalettedPerlinNoise::reset() {
      this->canvas->setRotate(Canvas::Rotate::None);
      this->canvas->setBrightness(255);
      this->frame = 0;
      Effect::reset();
    }

    const char * EffectPalettedPerlinNoise::getName() {
      return this->name;
    }

    void EffectPalettedPerlinNoise::loop() {
      Effect::loop();
      this->frame++;
      uint8_t width = this->canvas->getWidth(false);
      CRGB * leds = this->canvas->getLeds();
      for (uint8_t i = 0; i < this->canvas->getHeight(false); i++) {
        uint16_t ioffset = this->scale * i;
        for (uint8_t j = 0; j < width; j++) {
          uint16_t joffset = this->scale * j;
          uint8_t temp = inoise8(this->x + ioffset, this->y + joffset, this->z);
          this->data[i * width + j] = temp;
        }
      }
      this->z += this->speed;
      for (uint8_t i = 0; i < this->canvas->getHeight(false); i++) {
        for (uint8_t j = 0; j < width; j++) {
          //leds[this->canvas->XY(j, i)].setHSV(this->data[i * width + j], 255, this->data[j * width + i]);
          leds[this->canvas->XY(j, i)].setHSV(this->hue + (this->data[i * width + j]), 255, this->data[j * width + i]);
        }
      }
      this->x++;
      this->y--;
      if (--this->hueCounter == 0) {
        this->hue++;
        this->hueCounter = this->framePerSecond;
      }
      if (((uint32_t)this->frame * (1000 / this->framePerSecond)) >= this->timeInMilliseconds) {
        this->effectFinished();
      }
   }

  }
}