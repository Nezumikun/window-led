#include "with_time.h"
#include "../settings.h"

namespace Nezumikun {
  namespace WindowLed {

    EffectWithTime::EffectWithTime(Canvas & canvas, uint8_t framePerSecond, uint16_t timeInMilliseconds) : Effect(canvas, framePerSecond) {
      this->timeInMilliseconds = timeInMilliseconds;
    };

    void EffectWithTime::reset() {
      this->frame = 0;
      if (Settings::debugLevel >= DebugLevel::Debug) {
        Serial.println("EffectWithTime.reset");
      }
      Effect::reset();
    }

    void EffectWithTime::loop() {
      Effect::loop();
    }

  }
}
