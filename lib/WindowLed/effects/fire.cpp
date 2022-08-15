#include "fire.h"
#include "../settings.h"
namespace Nezumikun {
  namespace WindowLed {

    DEFINE_GRADIENT_PALETTE( firepal_p ) {    // define fire palette
      0,     0,  0,  0,   //black
      32,   255,  0,  0,  // red
      190, 255, 255, 0, //yellow
      255,   255, 255, 255  // white
    };

    EffectFire::EffectFire(Canvas & canvas, uint8_t framePerSecond, uint32_t timeInMilliseconds) : EffectWithTime (canvas, framePerSecond, timeInMilliseconds) {
      this->firstCall = true;
      this->myPal = firepal_p;
      this->data = (CRGB *) malloc(canvas.getLedsNumber() * sizeof(CRGB));
    };

    EffectFire::~EffectFire() {
      free(this->data);
    }

    const __FlashStringHelper * EffectFire::getName() {
      return F("Fire");
    }

    void EffectFire::reset() {
      this->canvas->setRotate(Canvas::Rotate::None);
      this->canvas->setBrightness(255);
      this->frame = 0;
      Effect::reset();
    }

    void EffectFire::loop() {
      Effect::loop();
      this->frame++;
      uint8_t width = this->canvas->getWidth();
      uint8_t height = this->canvas->getHeight();
      CRGB * leds = this->canvas->getLeds();
      for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
          leds[this->canvas->XY(x, height - y - 1)] = ColorFromPalette (myPal, qsub8 (inoise8 (x * 60 , y * 60+ a , a /3), 
            abs8(y - (height-1)) * 255 / (height-1)), 255);    
        }                                                              
      }      
      if (((uint32_t)this->frame * (1000 / this->framePerSecond)) >= this->timeInMilliseconds) {
        this->effectFinished();
      }
      this->a += 5;
   }

  }
}