#include "effects.h"

namespace Nezumikun {
  namespace WindowLed {

    Effect::Effect(Canvas & canvas, uint8_t framePerSecond) {
      Serial.println(__FUNCTION__);
      this->canvas = &canvas;
      this->framePerSecond = framePerSecond;
      Serial.print("  framePerSecond = ");
      Serial.println(this->framePerSecond);
    };

    bool Effect::endOfEffect() {
      return this->_endOfEffect;
    }

    EffectWithTime::EffectWithTime(Canvas & canvas, uint8_t framePerSecond, uint16_t timeInMilliseconds) : Effect(canvas, framePerSecond) {
      Serial.println(__FUNCTION__);
      this->timeInMilliseconds = timeInMilliseconds;
      Serial.print("  timeInMilliseconds = ");
      Serial.println(this->timeInMilliseconds);
    };

    void EffectRise::loop() {
      if (this->firstCall) {
        this->frame = 0;
        this->firstCall = false;
        this->_endOfEffect = false;
      }
      uint16_t currentTime = (uint16_t)this->frame * 1000 / this->framePerSecond;
      uint8_t angle = (uint8_t)(128 * currentTime / this->timeInMilliseconds) - 64;
      uint8_t brightness = ((uint16_t)sin8(angle) * 100 / 255);
      Serial.print("Frame = ");
      Serial.print(this->frame);
      Serial.print(" currentTime = ");
      Serial.print(currentTime);
      Serial.print(" Angle = ");
      Serial.print(angle);
      Serial.print(" value = ");
      Serial.print(brightness);
      Serial.println();
      this->canvas->setBrightness(brightness);
      if (currentTime >= this->timeInMilliseconds) {
        this->effectFinished();
      }
      this->frame++;
    };

    void EffectInit::loop() {
      if (this->firstCall) {
        this->_endOfEffect = false;
      }
    }

    void Effect::effectFinished() {
      this->_endOfEffect = true;
      this->firstCall = true;
    }

  }
}
