#include "fill.h"

namespace Nezumikun {
  namespace WindowLed {

    EffectFill::EffectFill(Canvas & canvas, uint8_t framePerSecond) : Effect (canvas, framePerSecond) {
      this->angleStep = (uint8_t)((uint16_t) 128 * 25 / 2 / framePerSecond);
      this->effectFade = new EffectFade(canvas, framePerSecond);
    };

    void EffectFill::reset() {
      this->x = 0;
      this->y = 0;
      if (this->direction == Effect::Direction::Random) {
        this->currentDirection = (random(255) & 0x01 == 1) ? Effect::Direction::Vertical : Effect::Direction::Horizontal;
      } else {
        this->currentDirection = this->direction;
      }
      if (this->fillColorStyle == Effect::FillColorStyle::RandomStyle) {
        this->currentFillColorStyle = (Effect::FillColorStyle) random(3);
      } else {
        this->currentFillColorStyle = this->fillColorStyle;
      }
      this->hue = random(255);
      this->dh = (random(255) & 0x01 == 1 ? -1 : 1) * (15 + random(30));
      this->canvas->setBrightness(255);
      this->riseEnd = false;
      this->canvas->fill(CRGB(0, 0, 0));
      this->effectFade->reset();
      this->fade = false;
      Effect::reset();
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
      this->canvas->getLeds()[this->canvas->XY(this->x, this->y)].setHSV(this->hue, 255, sin8(this->angle - 64));
      this->angle += this->angleStep;
      if ((this->angle >= 128) && this->riseEnd) {
        this->angle = 0;
        this->riseEnd = false;
        if (this->currentFillColorStyle == Effect::FillColorStyle::RandomColorForEachDot) {
          this->hue = random(255);
        }
        if (this->currentDirection == Effect::Direction::Horizontal) {
          if (++this->x >= this->canvas->getWidth()) {
            this->x = 0;
            if (++this->y >= this->canvas->getHeight()) {
              this->fade = true;
            }
          }
        } else {
          if (++this->y >= this->canvas->getHeight()) {
            this->y = 0;
            if (++this->x >= this->canvas->getWidth()) {
              this->fade = true;
            }
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