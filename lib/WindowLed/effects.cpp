#include "effects.h"

namespace Nezumikun {
  namespace WindowLed {

    Effect::Effect(Canvas & canvas) {
      this->canvas = &canvas;
    };

    bool Effect::endOfEffect() {
      return this->_endOfEffect;
    }

    void EffectInit::nextTick(bool firstCall) {
      if (firstCall) {
        this->step = 0;
        this->value = 0;
      }
    }

  }
}
