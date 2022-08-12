#ifndef WINDOWLED_EFFECT_FILL
#define WINDOWLED_EFFECT_FILL

#include "effect.h"
#include "fade.h"
#include "../helpers/dot.h"

namespace Nezumikun {
  namespace WindowLed {

    class EffectFill : public Effect {
      public:
        enum class FillStyle {
          NewLine,
          Continue,
          RandomStyle
        };
      private:
        void newLine();
      protected:
        Effect::Direction direction = Effect::Direction::Random;
        Effect::Direction currentDirection;
        Effect::FillColorStyle fillColorStyle = Effect::FillColorStyle::RandomStyle;
        Effect::FillColorStyle currentFillColorStyle;
        FillStyle fillStyle = FillStyle::RandomStyle;
        FillStyle currentFillStyle;
        EffectFade * effectFade;
        uint8_t x;
        uint8_t y;
        uint8_t hue;
        int8_t dh;
        uint8_t angleStep;
        uint8_t angle;
        Helpers::Dot * dot;
        bool riseEnd;
        bool fade;
        const char * name = "Fill";
      public:
        EffectFill(Canvas & canvas, uint8_t framePerSecond);
        ~EffectFill();
        virtual void reset() override;
        virtual void loop() override;
        virtual const char * getName() override;
    };

  }
}

#endif