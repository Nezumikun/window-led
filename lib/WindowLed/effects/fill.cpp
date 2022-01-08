#include "fill.h"
#include "../settings.h"
namespace Nezumikun {
  namespace WindowLed {

    EffectFill::EffectFill(Canvas & canvas, uint8_t framePerSecond) : Effect (canvas, framePerSecond) {
      this->angleStep = (uint8_t)((uint16_t) 128 * 25 / 2 / framePerSecond);
      this->effectFade = new EffectFade(canvas, framePerSecond);
      this->reset();
    };

    void EffectFill::reset() {
      this->x = 0;
      this->y = 0;
      if (this->direction == Effect::Direction::Random) {
        this->currentDirection = ((random(255) & 0x01) == 1) ? Effect::Direction::Vertical : Effect::Direction::Horizontal;
      } else {
        this->currentDirection = this->direction;
      }
      if (this->fillColorStyle == Effect::FillColorStyle::RandomStyle) {
        this->currentFillColorStyle = (Effect::FillColorStyle) random(3);
      } else {
        this->currentFillColorStyle = this->fillColorStyle;
      }
      if (this->fillStyle == FillStyle::RandomStyle) {
        this->currentFillStyle = (FillStyle) (random(255) & 0x01);
      } else {
        this->currentFillStyle = this->fillStyle;
      }
      this->hue = random(255);
      this->dh = ((random(255) & 0x01) == 1 ? -1 : 1) * (15 + random(30));
      this->canvas->setBrightness(255);
      this->riseEnd = false;
      this->canvas->fill(CRGB(0, 0, 0));
      // this->effectFade->reset();
      this->fade = false;
      Effect::reset();
      if (Settings::debugLevel >= DebugLevel::Debug) {
        Serial.print("EffectFill.reset");
        Serial.print(" Direction = ");
        Serial.print(this->currentDirection == Effect::Direction::Horizontal ? "Horizontal" : "Vertical");
        Serial.print(" FillColorStyle = ");
        Serial.print(this->currentFillColorStyle == FillColorStyle::SolidColor ? "SolidColor" : this->currentFillColorStyle == FillColorStyle::HueShift ? "HueShift" : "RandomColorForEachDot");
        Serial.print(" FillStyle = ");
        Serial.print(this->currentFillStyle == FillStyle::NewLine ? "NewLine" : "Continue");
        Serial.println();
      }
    }

    void EffectFill::newLine() {
      if (this->currentFillColorStyle == FillColorStyle::HueShift) {
        this->hue += this->dh;
      }
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
      if (Settings::debugLevel >= DebugLevel::Debug) {
      //if (this->currentFillStyle == FillStyle::Continue) {
        Serial.print("EffectFill.loop");
        Serial.print(" x = ");
        Serial.print(this->x);
        Serial.print(" (width = ");
        Serial.print(this->canvas->getWidth());
        Serial.print(")");
        Serial.print(" y = ");
        Serial.print(this->y);
        Serial.print(" (height = ");
        Serial.print(this->canvas->getHeight());
        Serial.print(")");
        Serial.print(" hue = ");
        Serial.print(this->hue);
        Serial.println();
      }
      this->canvas->getLeds()[this->canvas->XY(this->x, this->y)].setHSV(this->hue, 255, sin8(this->angle - 64));
      this->angle += this->angleStep;
      if ((this->angle >= 128) && this->riseEnd) {
        this->angle = 0;
        this->riseEnd = false;
        if (this->currentFillColorStyle == Effect::FillColorStyle::RandomColorForEachDot) {
          this->hue = random(255);
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
      else if ((this->angle >= 128) && !this->riseEnd) {
        this->angle = 128;
        this->riseEnd = true;
      }
    }
  }
}