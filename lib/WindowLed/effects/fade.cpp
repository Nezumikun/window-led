#include "fade.h"
#include "../settings.h"

namespace Nezumikun {
  namespace WindowLed {

    void EffectFade::loop() {
      EffectWithTime::loop();
      uint32_t currentTime = (uint32_t)this->frame * 1000 / this->framePerSecond;
      uint8_t angle = (uint8_t)(currentTime * 128 / this->timeInMilliseconds);
      // uint8_t brightness = ((uint16_t)cos8(angle) * 100 / 255);
      uint8_t brightness = cos8(angle);
      if (Settings::debugLevel >= DebugLevel::Debug) {
        Serial.print(F("EffectFade.loop :: Frame = "));
        Serial.print(this->frame);
        Serial.print(F(" currentTime = "));
        Serial.print(currentTime);
        Serial.print(F(" Angle = "));
        Serial.print(angle);
        Serial.print(F(" value = "));
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
