#include "init.h"
#include "../settings.h"

namespace Nezumikun {
  namespace WindowLed {

    EffectInit::EffectInit(Canvas & canvas, uint8_t framePerSecond, uint16_t timeInMilliseconds) : EffectWithTime(canvas, framePerSecond, timeInMilliseconds) {
      this->effectRise = new EffectRise(canvas, framePerSecond, timeInMilliseconds);
      this->effectFade = new EffectFade(canvas, framePerSecond, timeInMilliseconds);
      this->reset();
      this->firstCall = true;
    };

    EffectInit::~EffectInit() {
      delete this->effectFade;
      delete this->effectRise;
    };

    void EffectInit::reset() {
      this->mode = Mode::Red;
      this->subMode = SubMode::Draw;
      this->randomInfo.index = 0;
      this->randomInfo.hue = random(255);
      this->randomInfo.brightness = 0;
      if (Settings::debugLevel >= DebugLevel::Debug) {
        Serial.print(F("EffectInit.reset"));
      }
      EffectWithTime::reset();
    }

    void EffectInit::loop() {
      EffectWithTime::loop();
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
              Serial.print(F("EffectInit.loop :: Mode = Random"));
              Serial.print(F(" Index = "));
              Serial.print(this->randomInfo.index);
              Serial.print(F(" Hue = "));
              Serial.print(this->randomInfo.hue);
              Serial.print(F(" Brightness = "));
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
            Serial.print(F("EffectInit.loop :: fill "));
            Serial.println((this->mode == Mode::Red) ? F("Red")
              : (this->mode == Mode::Green) ? F("Green")
              : (this->mode == Mode::Blue) ? F("Blue")
              : (this->mode == Mode::White) ? F("White")
              : F("Black")
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
          if (Settings::debugLevel >= DebugLevel::Debug) {
            Serial.print(F("EffectInit.loop :: "));
            Serial.print((this->mode == Mode::Red) ? F("Red")
              : (this->mode == Mode::Green) ? F("Green")
              : (this->mode == Mode::Blue) ? F("Blue")
              : (this->mode == Mode::White) ? F("White")
              : F("Black")
            );
            Serial.println(F(" fade end"));
          }
          this->mode = (this->mode == Mode::Red) ? Mode::Green
            : (this->mode == Mode::Green) ? Mode::Blue
            : (this->mode == Mode::Blue) ? Mode::White
            : Mode::End
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
