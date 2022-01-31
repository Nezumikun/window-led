#include "dot.h"

namespace Nezumikun {
  namespace WindowLed {
    namespace Helpers {

      Dot::Dot (uint8_t framePerSecond, uint32_t timeInMilliseconds) {
        this->framePerSecond = framePerSecond;
        this->timeInMilliseconds = timeInMilliseconds;
        this->frameLimit = timeInMilliseconds / framePerSecond - 1;
        this->originalColor = CRGB(0, 0, 0);
      };

      void Dot::setColor(CRGB color) {
        this->originalColor = color;
      }

      void Dot::setColor(uint8_t hue) {
        this->originalColor.setHue(hue);
      }

      CRGB Dot::getColor() {
        return this->color;
      }

      uint8_t Dot::getBrightness() {
        return this->brightness;
      }

      bool Dot::isLastCall () {
        if (this->frame++ == this->frameLimit) {
          this->lastCall = true;
          this->firstCall = true;
        }
        return this->lastCall;
      }

      bool Dot::endOfEffect() {
        return this->lastCall;
      }

      bool Dot::isFirstCall() {
        if (this->firstCall) {
          this->firstCall = false;
          this->lastCall = false;
          this->frame = 0;
          return true;
        }
        return false;
      }

      CRGB Dot::rise() {
        this->isFirstCall();
        uint8_t angle = (uint8_t)((uint32_t)this->frame * 128 / this->frameLimit) - 64;
        this->brightness = sin8(angle);
        this->color = this->originalColor;
        this->color.fadeToBlackBy(255 - this->brightness);
        this->isLastCall();
        return this->color;
      }

      CRGB Dot::fade() {
        this->isFirstCall();
        uint8_t angle = (uint8_t)((uint32_t)this->frame * 128 / this->frameLimit);
        this->brightness = cos8(angle);
        this->color = this->originalColor;
        this->color.fadeToBlackBy(255 - this->brightness);
        this->isLastCall();
        return this->color;
      }

    }
  }
}
