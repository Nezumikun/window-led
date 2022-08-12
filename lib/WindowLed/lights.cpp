#include "lights.h"

namespace Nezumikun {
  namespace WindowLed {

    Lights::Lights(CRGB * leds, uint16_t ledsNumber, uint8_t width, uint8_t height, uint8_t framePerSecond) {
      this->leds = leds;
      this->ledsNumber = ledsNumber;
      this->framePerSecond = framePerSecond;
      this->canvas = new Canvas(width, height);
      this->effectInit = new EffectInit(*canvas, framePerSecond, timeInMilliseconds * 2);
      this->effectRise = new EffectRise(*canvas, framePerSecond, timeInMilliseconds);
      this->effectFade = new EffectFade(*canvas, framePerSecond, timeInMilliseconds);
      this->effectCount = 3;
      this->effects = (Effect **) malloc(this->effectCount * sizeof(void *));
      this->effects[0] = new EffectFill(*canvas, framePerSecond);
      this->effects[1] = new EffectPerlinNoise(*canvas, framePerSecond, 30 * 1000);
      this->effects[2] = new EffectPalettedPerlinNoise(*canvas, framePerSecond, 30 * 1000);
      this->mode = Mode::Init;
      this->saveMode = this->mode;
      this->startAt = StartAt::BottomLeft;
      this->linesDirection = LinesDirection::Vertical;
      for (uint16_t i = 0; i < ledsNumber; i++) {
        leds[i] = CRGB::Black;
      }
      this->state = State::On;
    }

    void Lights::setSkipInfo(SkipInfo * skip, uint8_t count) {
      this->skip = skip;
      this->skipCount = count;
      if (Settings::debugLevel >= DebugLevel::Debug) {
        Serial.print(F("Lights.setSkipInfo :: Skip count = "));
        Serial.println(this->skipCount);
        for (uint8_t i = 0; i < this->skipCount; i++) {
          Serial.print(F(" start = "));
          Serial.print(this->skip[i].start);
          Serial.print(F(" length = "));
          Serial.print(this->skip[i].length);
          Serial.println();
        }
      }
    }

    void Lights::setStartAt(StartAt startAt) {
      this->startAt = startAt;
    }

    void Lights::setLinesDirectoin(LinesDirection linesDirection) {
      this->linesDirection = linesDirection;
      if (Settings::debugLevel >= DebugLevel::Debug) {
        Serial.print(F("Lights::setLinesDirectoin"));
        Serial.println(this->linesDirection == LinesDirection::Horizontal ? F("Hotizontal") : F("Vertical"));
      }
    }


    uint16_t Lights::XY(uint8_t x, uint8_t y) {
      uint8_t tx = x;
      uint8_t ty = y;
      uint8_t tw = this->canvas->getWidth(false);
      uint8_t th = this->canvas->getHeight(false);
      uint16_t p = 0;
      if (this->startAt == StartAt::BottomLeft && linesDirection == LinesDirection::Horizontal) {
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
      uint8_t width = this->canvas->getWidth(false);
      for (uint8_t y = 0; y < this->canvas->getHeight(false); y++) {
        for (uint8_t x = 0; x < width; x++) {
          // Serial.println(String("x = ") + x + " y = " + y);
          this->leds[this->XY(x, y)] = canvasLeds[(uint16_t)y * width + x];
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
#ifdef DEBUG
        this->mode = Mode::Demo;
        this->currentEffect = 0;
#else
        this->mode = Mode::Init;
#endif
      }
      switch (this->mode) {
        case Mode::Init:
          this->effectInit->loop();
          if (this->effectInit->endOfEffect()) {
            delete this->effectInit;
            this->mode = Mode::Demo;
            this->currentEffect = 0;
          }
          break;
        case Mode::Demo:
          this->effects[this->currentEffect]->loop();
          if (this->effects[this->currentEffect]->endOfEffect()) {
            if ((random8() & 0x01) == 0) {
              uint8_t temp = 0;
              do { 
                temp = random8(this->effectCount);
              } while (temp == this->currentEffect);
              Serial.print("Change effect from ");
              Serial.print(this->currentEffect);
              Serial.print(": ");
              Serial.print(this->effects[this->currentEffect]->getName());
              Serial.print(" to ");
              Serial.print(temp);
              Serial.print(": ");
              Serial.print(this->effects[temp]->getName());
              Serial.println();
              this->currentEffect = temp;
            } else {
              Serial.println("Not change effect");
            }
          }
          break;
        case Mode::Off:
          this->effectFade->loop();
          if (this->effectFade->endOfEffect()) {
            this->mode = Mode::Wait;
            this->canvas->setBrightness(0);
          }
          break;
        case Mode::On:
          this->effectRise->loop();
          if (this->effectRise->endOfEffect()) {
            this->mode = this->saveMode;
            this->canvas->setBrightness(255);
          }
          break;
        case Mode::Wait:
          break;
      }
    }

    void Lights::on() {
      if (this->state == State::Off) {
        this->mode = Mode::On;
        this->state = State::On;
        this->effectRise->reset();
      }
    }

    void Lights::off() {
      if (this->state == State::On) {
        this->saveMode = this->mode;
        this->mode = Mode::Off;
        this->state = State::Off;
        this->effectFade->reset();
      }
    }

    bool Lights::isOn() {
      return this->state == State::On;
    }

  }
}
