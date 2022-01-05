#include "effect.h"
#include "../settings.h"
namespace Nezumikun {
  namespace WindowLed {

    Effect::Effect(Canvas & canvas, uint8_t framePerSecond) {
      this->canvas = &canvas;
      this->framePerSecond = framePerSecond;
    };

    Effect::~Effect() {
    };

    void Effect::setup() {
      if (Settings::debugLevel >= DebugLevel::Debug) {
        Serial.print("Effect.setup");
      }
    };

    bool Effect::endOfEffect() {
      return this->_endOfEffect;
    };

    void Effect::effectFinished() {
      this->_endOfEffect = true;
      this->firstCall = true;
      this->setup();
    };

  }
}
