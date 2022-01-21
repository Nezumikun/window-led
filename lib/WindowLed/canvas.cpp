#include "canvas.h"
#include "settings.h"

namespace Nezumikun {
  namespace WindowLed {

    Canvas::Canvas(uint8_t width, uint8_t height) {
      this->width = width;
      this->height = height;
      this->ledsNumber = (uint16_t)width * height;
      this->leds = (CRGB *) malloc((width * height) * sizeof(CRGB));
      this->anngle = Rotate::None;
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
      if (Settings::debugLevel >= DebugLevel::Debug) {
        Serial.print(F("Canvas.fill :: R = "));
        Serial.print(color.r);
        Serial.print(F(" G = "));
        Serial.print(color.g);
        Serial.print(F("B = "));
        Serial.print(color.b);
        Serial.println();
      }
      fill_solid(this->leds, this->ledsNumber, color);
    }

    uint8_t Canvas::X(uint8_t x, uint8_t y, bool useRotateSettings) {
      if (x >= this->getWidth(useRotateSettings)) {
        Serial.print(F("X more then canvas width! "));
        Serial.print(x);
        Serial.print(F(" >= "));
        Serial.println(this->getWidth(useRotateSettings));
        x = this->getWidth(useRotateSettings) - 1;
      }
      if (y >= this->getHeight(useRotateSettings)) {
        Serial.print(F("Y more then canvas height! "));
        Serial.print(y);
        Serial.print(F(" >= "));
        Serial.println(this->getHeight(useRotateSettings));
        y = this->getHeight(useRotateSettings) - 1;
      }
      return (this->anngle == Rotate::Angle90) ? y
        : (this->anngle == Rotate::Angle180) ? this->width - 1 - x
        : (this->anngle == Rotate::Angle270) ? this->width - 1 - y
        : x;
    }

    uint8_t Canvas::Y(uint8_t x, uint8_t y, bool useRotateSettings) {
      if (x >= this->getWidth(useRotateSettings)) {
        Serial.print(F("X more then canvas width! "));
        Serial.print(x);
        Serial.print(F(" >= "));
        Serial.println(this->getWidth(useRotateSettings));
        x = this->getWidth(useRotateSettings) - 1;
      }
      if (y >= this->getHeight(useRotateSettings)) {
        Serial.print(F("Y more then canvas height! "));
        Serial.print(y);
        Serial.print(F(" >= "));
        Serial.println(this->getHeight(useRotateSettings));
        y = this->getHeight(useRotateSettings) - 1;
      }
      return (this->anngle == Rotate::Angle90) ? this->height - 1 - x
        : (this->anngle == Rotate::Angle180) ? this->height - 1 - y
        : (this->anngle == Rotate::Angle270) ? x
        : y;
    }

    uint16_t Canvas::XY(uint8_t x, uint8_t y, bool useRotateSettings) {
      return (uint16_t)this->Y(x, y, useRotateSettings) * this->width + this->X(x, y, useRotateSettings);
    }

    void Canvas::setRotate(Rotate angle) {
      this->anngle = angle;
    }

    void Canvas::setRotateRandom() {
      this->anngle = (Rotate) (random(255) & 0x03);
    }

    Canvas::Rotate Canvas::getRotate() {
      return this->anngle;
    }

  }
}