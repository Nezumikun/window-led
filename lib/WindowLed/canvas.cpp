#include "canvas.h"

namespace Nezumikun {
  namespace WindowLed {

    Canvas::Canvas(uint8_t width, uint8_t height) {
      this->width = width;
      this->height = height;
      this->leds = (CRGB *) malloc((width * height) * sizeof(CRGB));
    }

    uint8_t Canvas::getWidth() {
      return this->width;
    };

    uint8_t Canvas::getHeight() {
      return this->height;
    };

    uint8_t Canvas::getBrightness() {
      return this->brightness;
    };

    void Canvas::setBrightness(uint8_t brightness) {
      this->brightness = brightness;
    };

    CRGB * Canvas::getLeds() {
      return this->leds;
    };

  }
}