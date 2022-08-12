#include "fill.h"
#include "../settings.h"
namespace Nezumikun {
  namespace WindowLed {

    EffectFill::EffectFill(Canvas & canvas, uint8_t framePerSecond) : Effect (canvas, framePerSecond) {
      this->angleStep = (uint8_t)((uint32_t) 128 * 25 / 4 / framePerSecond);
      this->effectFade = new EffectFade(canvas, framePerSecond);
      this->dot = new Helpers::Dot(framePerSecond, 500);
      this->reset();
      this->firstCall = true;
    };

    EffectFill::~EffectFill() {
      delete this->effectFade;
      delete this->dot;
    }

    void EffectFill::reset() {
      this->x = 0;
      this->y = 0;
      if (this->direction == Effect::Direction::Random) {
        this->currentDirection = ((random8(255) & 0x01) == 1) ? Effect::Direction::Vertical : Effect::Direction::Horizontal;
      } else {
        this->currentDirection = this->direction;
      }
      if (this->fillColorStyle == Effect::FillColorStyle::RandomStyle) {
        this->currentFillColorStyle = (Effect::FillColorStyle) random8(3);
      } else {
        this->currentFillColorStyle = this->fillColorStyle;
      }
      if (this->fillStyle == FillStyle::RandomStyle) {
        this->currentFillStyle = (FillStyle) (random8(255) & 0x01);
      } else {
        this->currentFillStyle = this->fillStyle;
      }
      this->hue = random8(255);
      this->dh = ((random8(255) & 0x01) == 1 ? -1 : 1) * (15 + random8(30));
      this->canvas->setBrightness(255);
      this->canvas->setRotateRandom();
      this->riseEnd = false;
      this->canvas->fill(CRGB(0, 0, 0));
      // this->effectFade->reset();
      this->fade = false;
      this->dot->setColor(this->hue);
      Effect::reset();
      if (Settings::debugLevel >= DebugLevel::Debug) {
        Serial.print(F("EffectFill.reset"));
        Serial.print(F(" Direction = "));
        Serial.print(this->currentDirection == Effect::Direction::Horizontal ? F("Horizontal") : F("Vertical"));
        Serial.print(F(" FillColorStyle = "));
        Serial.print(this->currentFillColorStyle == FillColorStyle::SolidColor ? F("SolidColor") : this->currentFillColorStyle == FillColorStyle::HueShift ? F("HueShift") : F("RandomColorForEachDot"));
        Serial.print(F(" FillStyle = "));
        Serial.print(this->currentFillStyle == FillStyle::NewLine ? F("NewLine") : F("Continue"));
        Serial.print(F(" CanvasRotate = "));
        Canvas::Rotate rotate = this->canvas->getRotate();
        Serial.print(rotate == Canvas::Rotate::None ? F("None") : rotate == Canvas::Rotate::Angle90 ? F("Angle90") : rotate == Canvas::Rotate::Angle180 ? F("Angle180") : F("Angle270"));
        Serial.println();
      }
    }

    void EffectFill::newLine() {
      if (this->currentFillColorStyle == FillColorStyle::HueShift) {
        this->hue += this->dh;
      }
    }

    const __FlashStringHelper * EffectFill::getName() {
      return F("Fill");
    }

    void EffectFill::loop() {
      Effect::loop();
      if (this->fade) {
        this->effectFade->loop();
        if (this->effectFade->endOfEffect()) {
          this->effectFinished();
        }
        return;
      }
      this->dot->setColor(this->hue);
      CRGB temp = this->dot->rise();
      this->canvas->getLeds()[this->canvas->XY(this->x, this->y)] = temp;
      if (Settings::debugLevel >= DebugLevel::Debug) {
      //if ((this->canvas->getRotate() == Canvas::Rotate::Angle90) || (this->canvas->getRotate() == Canvas::Rotate::Angle270)) {
        Serial.print(F("EffectFill.loop"));
        Serial.print(F(" x = "));
        Serial.print(this->x);
        Serial.print(F(" (width = "));
        Serial.print(this->canvas->getWidth());
        Serial.print(F(")"));
        Serial.print(F(" y = "));
        Serial.print(this->y);
        Serial.print(F(" (height = "));
        Serial.print(this->canvas->getHeight());
        Serial.print(F(")"));
        Serial.print(F(" hue = "));
        Serial.print(this->hue);
        Serial.print(F(" brightness = "));
        Serial.print(this->dot->getBrightness());
        Serial.print(F(" R = "));
        Serial.print(temp.r);
        Serial.print(F(" G = "));
        Serial.print(temp.g);
        Serial.print(F(" B = "));
        Serial.print(temp.b);
        Serial.print(F(" Canvas: x = "));
        Serial.print(this->canvas->X(x, y));
        Serial.print(F(" y = "));
        Serial.print(this->canvas->Y(x, y));
        Serial.println();
      }
      if (this->dot->endOfEffect()) {
        if (this->currentFillColorStyle == Effect::FillColorStyle::RandomColorForEachDot) {
          this->hue = random8(255);
        }
        if (this->currentDirection == Effect::Direction::Horizontal) {
          if (this->currentFillStyle == FillStyle::Continue) {
            if ((this->y & 0x01) == 0) {
              if (++this->x >= this->canvas->getWidth()) {
                this->x = this->canvas->getWidth() - 1;
                this->y++;
                this->newLine();
              }
            } else {
              if (--this->x == 255) {
                this->x = 0;
                this->y++;
                this->newLine();
              }
            }
          } else {
            if (++this->x >= this->canvas->getWidth()) {
              this->x = 0;
              this->y++;
              this->newLine();
            }
          }
          if (this->y >= this->canvas->getHeight()) {
            this->fade = true;
          }
        } else {
          if (this->currentFillStyle == FillStyle::Continue) {
            if ((this->x & 0x01) == 0) {
              if (++this->y >= this->canvas->getHeight()) {
                this->y = this->canvas->getHeight() - 1;
                this->x++;
                this->newLine();
              }
            } else {
              if (--this->y == 255) {
                this->y = 0;
                this->x++;
                this->newLine();
              }
            }
          } else {
            if (++this->y >= this->canvas->getHeight()) {
              this->y = 0;
              this->x++;
              this->newLine();
            }
          }
          if (this->x >= this->canvas->getWidth()) {
            this->fade = true;
          }
        }
      }
      //else if ((this->angle >= 128) && !this->riseEnd) {
      //  this->angle = 128;
      //  this->riseEnd = true;
      //}
    }
  }
}