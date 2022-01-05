#include "init.h"
#include "../settings.h"

namespace Nezumikun {
  namespace WindowLed {

    EffectInit::EffectInit(Canvas & canvas, uint8_t framePerSecond, uint16_t timeInMilliseconds) : EffectWithTime(canvas, framePerSecond, timeInMilliseconds) {
      this->effectRise = new EffectRise(canvas, framePerSecond, timeInMilliseconds);
      this->effectFade = new EffectFade(canvas, framePerSecond, timeInMilliseconds);
    };

    void EffectInit::loop() {
      if (this->firstCall) {
        this->_endOfEffect = false;
        this->firstCall = false;
        this->mode = EffectInit::Mode::Red;
        this->subMode = EffectInit::SubMode::Draw;
      }
      if (this->subMode == EffectInit::SubMode::Draw) {
        this->canvas->fill((this->mode == EffectInit::Mode::Red) ? CRGB(255, 0, 0)
          : (this->mode == EffectInit::Mode::Green) ? CRGB(0, 255, 0)
          : (this->mode == EffectInit::Mode::Blue) ? CRGB(0, 0, 255)
          : (this->mode == EffectInit::Mode::White) ? CRGB(255, 255, 255)
          : CRGB(0, 0, 0)
        );
        if (Settings::debugLevel >= DebugLevel::Debug) {
          Serial.print("EffectInit.loop :: fill ");
          Serial.println((this->mode == EffectInit::Mode::Red) ? "Red"
            : (this->mode == EffectInit::Mode::Green) ? "Green"
            : (this->mode == EffectInit::Mode::Blue) ? "Blue"
            : (this->mode == EffectInit::Mode::White) ? "White"
            : "Black"
          );
        }
        this->subMode = EffectInit::SubMode::Rise;
      }
      if (this->subMode == EffectInit::SubMode::Rise) {
        this->effectRise->loop();
        if (this->effectRise->endOfEffect()) {
          this->subMode = EffectInit::SubMode::Fade;
        }
      } else {
        this->effectFade->loop();
        if (this->effectFade->endOfEffect()) {
          this->mode = (this->mode == EffectInit::Mode::Red) ? EffectInit::Mode::Green
            : (this->mode == EffectInit::Mode::Green) ? EffectInit::Mode::Blue
            : (this->mode == EffectInit::Mode::Blue) ? EffectInit::Mode::White
            : EffectInit::Mode::End
          ;
          this->subMode = EffectInit::SubMode::Draw;
        }
      }
      if (this->mode == EffectInit::Mode::End) {
        this->_endOfEffect = true;
      }
    }

    void Effect::effectFinished() {
      this->_endOfEffect = true;
      this->firstCall = true;
    }

  }
}
