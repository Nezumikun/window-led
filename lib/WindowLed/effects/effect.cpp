#include "effect.h"

namespace Nezumikun {
  namespace WindowLed {

    Effect::Effect(Canvas & canvas, uint8_t framePerSecond) {
      this->canvas = &canvas;
      this->framePerSecond = framePerSecond;
    };

    Effect::~Effect() {
    };

    bool Effect::endOfEffect() {
      return this->_endOfEffect;
    };

  }
}
