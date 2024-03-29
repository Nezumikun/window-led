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

    void Effect::reset() {
      this->firstCall = false;
      this->_endOfEffect = false;
      if (Settings::debugLevel >= DebugLevel::Debug) {
        Serial.println(F("Effect.reset"));
      }
    }

    bool Effect::endOfEffect() {
      return this->_endOfEffect;
    };

    void Effect::effectFinished() {
      this->_endOfEffect = true;
      this->firstCall = true;
    };

    void Effect::loop() {
      if (this->firstCall) {
        this->reset();
      }
    }

    const __FlashStringHelper * Effect::getName() {
      return F("Unnamed effect");
    }

  }
}
