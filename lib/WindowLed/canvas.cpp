#include "canvas.h"

namespace Nezumikun {
  namespace WindowLed {

    Canvas::Canvas(uint8_t width, uint8_t height) {
      this->width = width;
      this->height = height;
      this->ledsNumber = (uint16_t)width * height;
      this->leds = (CRGB *) malloc((width * height) * sizeof(CRGB));
    }

    uint8_t Canvas::getWidth() {
      return this->width;
    };

    uint8_t Canvas::getHeight() {
      return this->height;
    };

    uint16_t Canvas::getLedsNumber() {
      return this->ledsNumber;
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

    void Canvas::fill(const struct CRGB & color) {
      fill_solid(this->leds, this->ledsNumber, color);
    }

    uint16_t Canvas::XY(uint8_t x, uint8_t y) {
      return (uint16_t)y * this->width + x;
    }

  }
}