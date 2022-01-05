#include "with_time.h"

namespace Nezumikun {
  namespace WindowLed {

    EffectWithTime::EffectWithTime(Canvas & canvas, uint8_t framePerSecond, uint16_t timeInMilliseconds) : Effect(canvas, framePerSecond) {
      this->timeInMilliseconds = timeInMilliseconds;
    };

  }
}
