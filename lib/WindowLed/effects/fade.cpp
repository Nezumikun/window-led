#include "fade.h"
#include "../settings.h"

namespace Nezumikun {
  namespace WindowLed {

    void EffectFade::loop() {
      if (this->firstCall) {
        this->frame = 0;
        this->firstCall = false;
        this->_endOfEffect = false;
      }
      uint16_t currentTime = (uint16_t)this->frame * 1000 / this->framePerSecond;
      uint8_t angle = (uint8_t)(128 * currentTime / this->timeInMilliseconds);
      // uint8_t brightness = ((uint16_t)cos8(angle) * 100 / 255);
      uint8_t brightness = cos8(angle);
      if (Settings::debugLevel >= DebugLevel::Debug) {
        Serial.print("EffectFade.loop :: Frame = ");
        Serial.print(this->frame);
        Serial.print(" currentTime = ");
        Serial.print(currentTime);
        Serial.print(" Angle = ");
        Serial.print(angle);
        Serial.print(" value = ");
        Serial.print(brightness);
        Serial.println();
      }
      this->canvas->setBrightness(brightness);
      if (currentTime >= this->timeInMilliseconds) {
        this->effectFinished();
      } else {
        this->frame++;
      }
    };

  }
}
