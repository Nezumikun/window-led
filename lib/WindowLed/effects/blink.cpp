#include "blink.h"
#include "../settings.h"
namespace Nezumikun {
  namespace WindowLed {

    EffectBlink::EffectBlink(Canvas & canvas, uint8_t framePerSecond, uint16_t timeInMilliseconds) : EffectWithTime (canvas, framePerSecond, timeInMilliseconds) {
      this->effectFade = new EffectFade(canvas, framePerSecond, timeInMilliseconds / 2);
      this->effectRise = new EffectRise(canvas, framePerSecond, timeInMilliseconds / 2);
      this->setRandomColor();
      this->firstCall = true;
    };

    EffectBlink::~EffectBlink() {
      delete this->effectRise;
      delete this->effectFade;
    }

    void EffectBlink::reset() {
      this->mode = Mode::Rise;
      this->effectRise->reset();
      this->effectFade->reset();
      this->canvas->fill(this->color);
      EffectWithTime::reset();
    }

    void EffectBlink::loop() {
      Effect::loop();
      if (this->mode == Mode::Rise) {
        this->effectRise->loop();
        if (this->effectRise->endOfEffect()) {
          this->mode = Mode::Fade;
        }
      }
      else if (this->mode == Mode::Fade) {
        this->effectFade->loop();
        if (this->effectFade->endOfEffect()) {
          this->effectFinished();
        }
        return;
      }
   }

   void EffectBlink::setColor(uint8_t hue) {
     this->color.setHue(hue);
   }

   void EffectBlink::setColor(CRGB color) {
     this->color = color;
   }

   void EffectBlink::setRandomColor() {
     this->setColor((uint8_t)random(255));
   }

  }
}