#include "init.h"
#include "../settings.h"

namespace Nezumikun {
  namespace WindowLed {

    EffectInit::EffectInit(Canvas & canvas, uint8_t framePerSecond, uint16_t timeInMilliseconds) : EffectWithTime(canvas, framePerSecond, timeInMilliseconds) {
      this->effectRise = new EffectRise(canvas, framePerSecond, timeInMilliseconds);
      this->effectFade = new EffectFade(canvas, framePerSecond, timeInMilliseconds);
      this->setup();
    };

    EffectInit::~EffectInit() {
      delete this->effectFade;
      delete this->effectRise;
    };

    void EffectInit::setup() {
      Effect::setup();
      this->randomInfo.index = 0;
      this->randomInfo.hue = random(255);
      this->randomInfo.brightness = 0;
      if (Settings::debugLevel >= DebugLevel::Debug) {
        Serial.print("EffectInit.setup");
      }
    }

    void EffectInit::loop() {
      if (this->firstCall) {
        this->_endOfEffect = false;
        this->firstCall = false;
        this->mode = Mode::Red;
        this->subMode = SubMode::Draw;
      }
      if (this->subMode == SubMode::Draw) {
        if (this->mode == Mode::Random) {
          if ((this->randomInfo.index == 0) && (this->randomInfo.brightness == 0)) {
            this->canvas->fill(CRGB(0, 0, 0));
            this->canvas->setBrightness(255);
          }
          if (this->randomInfo.index >= this->canvas->getLedsNumber()) {
            this->effectFade->loop();
            if (this->effectFade->endOfEffect()) {
              this->mode = Mode::End;
            }
          } else {
            this->randomInfo.brightness += 127;
            if (this->randomInfo.brightness >= 250) {
              this->randomInfo.brightness = 255;
            }
            this->canvas->getLeds()[this->randomInfo.index].setHSV(this->randomInfo.hue, 255, this->randomInfo.brightness);
            if (this->randomInfo.brightness == 255) {
              this->randomInfo.index++;
              this->randomInfo.hue = random(255);
              this->randomInfo.brightness = 0;
            }
            if (Settings::debugLevel >= DebugLevel::Debug) {
              Serial.print("EffectInit.loop :: Mode = Random");
              Serial.print(" Index = ");
              Serial.print(this->randomInfo.index);
              Serial.print(" Hue = ");
              Serial.print(this->randomInfo.hue);
              Serial.print(" Brightness = ");
              Serial.print(this->randomInfo.brightness);
              Serial.println();
            }
          }
        } else {
          this->canvas->fill((this->mode == Mode::Red) ? CRGB(255, 0, 0)
            : (this->mode == Mode::Green) ? CRGB(0, 255, 0)
            : (this->mode == Mode::Blue) ? CRGB(0, 0, 255)
            : (this->mode == Mode::White) ? CRGB(255, 255, 255)
            : CRGB(0, 0, 0)
          );
          if (Settings::debugLevel >= DebugLevel::Debug) {
            Serial.print("EffectInit.loop :: fill ");
            Serial.println((this->mode == Mode::Red) ? "Red"
              : (this->mode == Mode::Green) ? "Green"
              : (this->mode == Mode::Blue) ? "Blue"
              : (this->mode == Mode::White) ? "White"
              : "Black"
            );
          }
          this->subMode = SubMode::Rise;
        }
      }
      if (this->subMode == SubMode::Rise) {
        this->effectRise->loop();
        if (this->effectRise->endOfEffect()) {
          this->subMode = SubMode::Fade;
        }
      } else if (this->subMode == SubMode::Fade) {
        this->effectFade->loop();
        if (this->effectFade->endOfEffect()) {
          this->mode = (this->mode == Mode::Red) ? Mode::Green
            : (this->mode == Mode::Green) ? Mode::Blue
            : (this->mode == Mode::Blue) ? Mode::White
            : Mode::Random
          ;
          this->subMode = SubMode::Draw;
        }
      }
      if (this->mode == Mode::End) {
        this->effectFinished();
      }
    }

  }
}
