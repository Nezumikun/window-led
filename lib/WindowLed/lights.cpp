#include "lights.h"

namespace Nezumikun {
  namespace WindowLed {

    Lights::Lights(CRGB * leds, uint16_t ledsNumber, uint8_t width, uint8_t height) {
      this->leds = leds;
      this->ledsNumber = ledsNumber;
      this->canvas = new Canvas(width, height);
      this->effectInit = new EffectInit(*canvas, framePerSecond, timeInMilliseconds);
      this->effectRise = new EffectRise(*canvas, framePerSecond, timeInMilliseconds);
      this->effectFade = new EffectFade(*canvas, framePerSecond, timeInMilliseconds);
      this->mode = Mode::Init;
      this->startAt = StartAt::BottomLeft;
      this->linesDirection = LinesDirection::Vertical;
      for (uint16_t i = 0; i < ledsNumber; i++) {
        leds[i] = CRGB::Black;
      }
    }

    void Lights::setSkipInfo(SkipInfo * skip, uint8_t count) {
      this->skip = skip;
      this->skipCount = count;
      if (Settings::debugLevel >= DebugLevel::Debug) {
        Serial.print("Lights.setSkipInfo :: Skip count = ");
        Serial.println(this->skipCount);
        for (uint8_t i = 0; i < this->skipCount; i++) {
          Serial.print(" start = ");
          Serial.print(this->skip[i].start);
          Serial.print(" length = ");
          Serial.print(this->skip[i].length);
          Serial.println();
        }
      }
    }

    uint16_t Lights::XY(uint8_t x, uint8_t y) {
      uint8_t tx = x;
      uint8_t ty = y;
      uint8_t tw = this->canvas->getWidth();
      uint8_t th = this->canvas->getHeight();
      uint16_t p = 0;
      if (this->startAt == StartAt::BottomLeft && linesDirection == LinesDirection::Hotizontal) {
        p = ty * tw + (((ty & 1) == 1) ? (tw - tx - 1) : tx);
      }
      else if (this->startAt == StartAt::BottomRight && linesDirection == LinesDirection::Vertical) {
        ty = tw - x - 1;
        tx = y;
        tw = th;
        p = ty * tw + (((ty & 1) == 1) ? (tw - tx - 1) : tx);
      }
      else if (this->startAt == StartAt::BottomLeft && linesDirection == LinesDirection::Vertical) {
        tw = th;
        p = tx * tw + (((tx & 1) == 1) ? (tw - ty - 1) : ty);
      }
      for (uint8_t i = 0; i < this->skipCount; i++) {
        if (p >= this->skip[i].start) {
          p += this->skip[i].length;
        }
        else {
          break;
        }
      }
      return p;
    }

    void Lights::show() {
      CRGB * canvasLeds = this->canvas->getLeds();
      uint8_t width = this->canvas->getWidth();
      for (uint8_t y = 0; y < this->canvas->getHeight(); y++) {
        for (uint8_t x = 0; x < width; x++) {
          // Serial.println(String("x = ") + x + " y = " + y);
          this->leds[this->XY(x, y)] = canvasLeds[y * width + x];
        }
      }
      for (uint8_t i = 0; i < this->skipCount; i++) {
        for (uint16_t p = this->skip[i].start; p < this->skip[i].start + this->skip[i].length; p++) {
          this->leds[p] = CRGB::Black;
        }
      }
      FastLED.show(this->canvas->getBrightness());
    }

    void Lights::loop() {
      this->show();
      if (this->firstCall) {
        this->firstCall = false;
        this->mode = Mode::Init;
      }
      if (this->mode == Mode::Init) {
        this->effectInit->loop();
        if (this->effectInit->endOfEffect()) {
          delete this->effectInit;
          this->mode = Mode::Mode1;
        }
      }
    }

    void Lights::on() {
      if (this->state == State::Off) {
        this->mode = Mode::On;
      }
    }

    void Lights::off() {
      if (this->state == State::On) {
        this->saveMode = this->mode;
        this->mode = Mode::Off;
      }
    }

    bool Lights::isOn() {
      return this->state == State::On;
    }

  }
}