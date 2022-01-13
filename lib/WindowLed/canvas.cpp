#include "canvas.h"

namespace Nezumikun {
  namespace WindowLed {

    Canvas::Canvas(uint8_t width, uint8_t height) {
      this->width = width;
      this->height = height;
      this->ledsNumber = (uint16_t)width * height;
      this->leds = (CRGB *) malloc((width * height) * sizeof(CRGB));
    }

    uint8_t Canvas::getWidth(bool useRotateSettings) {
      return (!useRotateSettings || (this->anngle == Rotate::None) || (this->anngle == Rotate::Angle180)) ? this->width : this->height;
    };

    uint8_t Canvas::getHeight(bool useRotateSettings) {
      return (!useRotateSettings || (this->anngle == Rotate::None) || (this->anngle == Rotate::Angle180)) ? this->height : this->width;
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

    uint16_t Canvas::XY(uint8_t x, uint8_t y, bool useRotateSettings) {
      if (x >= this->getWidth(useRotateSettings)) {
        Serial.print("X more then canvas width! ");
        Serial.print(x);
        Serial.print(" >= ");
        Serial.println(this->getWidth(useRotateSettings));
        x = this->getWidth(useRotateSettings) - 1;
      }
      if (y >= this->getHeight(useRotateSettings)) {
        Serial.print("Y more then canvas height! ");
        Serial.print(y);
        Serial.print(" >= ");
        Serial.println(this->getHeight(useRotateSettings));
        y = this->getHeight(useRotateSettings) - 1;
      }
      return (this->anngle == Rotate::Angle90) ? (uint16_t)(this->height - 1 - x) * this->width + y
        : (this->anngle == Rotate::Angle180) ? (uint16_t)(this->height - 1 - y) * this->width + (this->width - 1 - x)
        : (this->anngle == Rotate::Angle270) ? (uint16_t)x * this->width + (this->width - 1 - y)
        : (uint16_t)y * this->width + x;
    }

    void Canvas::setRotate(Rotate angle) {
      this->anngle = angle;
    }

    void Canvas::setRotateRandom() {
      this->anngle = (Rotate) (random(255) & 0x03);
    }

  }
}