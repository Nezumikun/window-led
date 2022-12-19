#include "init.h"
#include "../settings.h"

namespace Nezumikun {
  namespace WindowLed {

    EffectInit::EffectInit(Canvas & canvas, uint8_t framePerSecond, uint32_t timeInMilliseconds) : EffectWithTime(canvas, framePerSecond, timeInMilliseconds) {
      this->effectBlink = new EffectBlink(canvas, framePerSecond, timeInMilliseconds * 2);
      this->effectFade = new EffectFade(canvas, framePerSecond, timeInMilliseconds);
      this->reset();
      this->firstCall = true;
    };

    EffectInit::~EffectInit() {
      delete this->effectFade;
      delete this->effectBlink;
    };

    void EffectInit::reset() {
      this->mode = Mode::Red;
      this->subMode = SubMode::Draw;
      if (Settings::debugLevel >= DebugLevel::Debug) {
        Serial.print(F("EffectInit.reset"));
      }
      EffectWithTime::reset();
    }

    void EffectInit::loop() {
      EffectWithTime::loop();
      if (this->subMode == SubMode::Draw) {
        this->effectBlink->setColor((this->mode == Mode::Red) ? CRGB(255, 0, 0)
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
        this->subMode = SubMode::Blink;
      }
      if (this->subMode == SubMode::Blink) {
        this->effectBlink->loop();
        if (this->effectBlink->endOfEffect()) {
          if (Settings::debugLevel >= DebugLevel::Debug) {
            Serial.print(F("EffectInit.loop :: "));
            Serial.print((this->mode == Mode::Red) ? F("Red")
              : (this->mode == Mode::Green) ? F("Green")
              : (this->mode == Mode::Blue) ? F("Blue")
              : (this->mode == Mode::White) ? F("White")
              : F("Black")
            );
            Serial.println(F(" blink end"));
          }
          this->mode = (this->mode == Mode::Red) ? Mode::Green
            : (this->mode == Mode::Green) ? Mode::Blue
            // : (this->mode == Mode::Blue) ? Mode::White
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
