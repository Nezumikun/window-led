#include "WindowLed.h"

namespace Nezumikun {
  namespace WL {

    WindowLed::WindowLed(uint8_t LedsNumber, uint8_t controlPin, SKIP_INFO* skip, uint8_t skipCount, uint8_t width, uint8_t height, uint8_t startPoint, uint8_t linesDirection) {
      this->LedsNumber = LedsNumber;
      this->skip = skip;
      this->skipCount = skipCount;
      this->width = width;
      this->height = height;
      this->startPoint = startPoint;
      this->linesDirection = linesDirection;
      this->isOn = true;
      this->leds = (CRGB *) malloc(LedsNumber * sizeof(CRGB));
      this->controlPin = controlPin;
      this->ledsHue = (uint8_t *) malloc(LedsNumber * sizeof(uint8_t));
      effectInfo list[] = {
        { WINDOWLED_INIT, 1 },
        { WINDOWLED_FILL, -6 },
        { WINDOWLED_TOWARDS, -6 },
        { WINDOWLED_SQARESNAKE, -5},
        { WINDOWLED_FADE, 1 },
        { WINDOWLED_RAINBOW, -2 },
        { WINDOWLED_DROP, 1 },
        { WINDOWLED_RAINBOW, -2 }
      };
      this->effectCount = sizeof(list)/sizeof(effectInfo);
      this->effectList = (effectInfo *) malloc(this->effectCount * sizeof(effectInfo));
      memcpy(this->effectList, list, this->effectCount * sizeof(effectInfo));
      FastLED.addLeds<WS2812B, D5, GRB>(leds, (int)LedsNumber);  // GRB ordering is typical
    }

    void WindowLed::begin() {
      for (int i = 0; i < LedsNumber; i++) {
        leds[i] = CRGB::Black;
      }
      randomSeed(analogRead(0));
      this->currentEffect = 0;
      this->currentTime = 1;
      showCurrentSequence(millis());
    }

    int WindowLed::XY(uint8_t x, uint8_t y) {
      uint8_t tx = x;
      uint8_t ty = y;
      uint8_t tw = width;
      int p = 0;
      if (startPoint == START_AT_BOTTOMLEFT && linesDirection == LINES_HORIZONTAL) {
        p = ty * tw + (((ty & 1) == 1) ? (tw - tx - 1) : tx);
      }
      else if (startPoint == START_AT_BOTTOMRIGHT && linesDirection == LINES_VERTICAL) {
        ty = width - x - 1;
        tx = y;
        tw = height;
        p = ty * tw + (((ty & 1) == 1) ? (tw - tx - 1) : tx);
      }
      else if (startPoint == START_AT_BOTTOMLEFT && linesDirection == LINES_VERTICAL) {
        tw = height;
        p = tx * tw + (((tx & 1) == 1) ? (tw - ty - 1) : ty);
      }
      for (uint8_t i = 0; i < this->skipCount; i++) {
        if (p >= skip[i].start) {
          p += skip[i].length;
        }
        else {
          break;
        }
      }
      return p;
    }

    void WindowLed::setHueXY(uint8_t x, uint8_t y, uint8_t hue) {
      leds[XY(x, y)].setHue(hue);
    }

    void WindowLed::setHsvXY(uint8_t x, uint8_t y, uint8_t hue, uint8_t sat, uint8_t value) {
      leds[XY(x, y)].setHSV(hue, sat, value);
    }

    void WindowLed::setRgbXY(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
      leds[XY(x, y)].setRGB(r, g, b);
    }

    void WindowLed::copyXY(uint8_t fromX, uint8_t fromY, uint8_t toX, uint8_t toY) {
      uint8_t from = XY(fromX, fromY);
      leds[XY(toX, toY)].setRGB(leds[from].r, leds[from].g, leds[from].b);
    }

    void WindowLed::fillRgb(uint8_t r, uint8_t g, uint8_t b) {
      for (int i = 0; i < this->LedsNumber; i++) {
        leds[i].setRGB(r, g, b);
      }
    }

    bool WindowLed::effectRainbow(bool firstStart) {
      static uint8_t hue = 0;
      const uint8_t step = 20;
      const uint8_t shift = 1;
      if (firstStart) {
        hue = 0;
        Serial.println(__FUNCTION__);
      }
      for (uint8_t x = 0; x < width; x++) {
        for (uint8_t y = 0; y < height; y++) {
          setHueXY(width - x - 1, y, hue + x * step);
        }
      }
      bool retVal = !(hue + shift > 255);
      hue += shift;
      return retVal;
    }

    bool WindowLed::effectDrop(bool firstStart) {
      static DROP_PARAMS *params = NULL;
      FastLED.clearData();
      uint8_t count = 0;
      uint8_t hue = 0;
      if (firstStart) {
        if (params == NULL) {
          params = (DROP_PARAMS *) malloc(width * sizeof(DROP_PARAMS));
        }
        hue = random(256);
        for (uint8_t i = 0; i < width; i++) {
          params[i].inc = (random(3) + 3) * 25 ;
          params[i].y = 0;
          params[i].speed = 0;
          params[i].height = 0;
          params[i].amount = 0;
          params[i].hue = hue;
        }
        count = width;
        Serial.println(__FUNCTION__);
      }
      else {
        for (uint8_t x = 0; x < width; x++) {
          if (params[x].height == height) {
          } else {
            if (params[x].y <= params[x].height) {
              params[x].y = height - 1;
            }
            else {
              params[x].y--;
            }
            if (params[x].y == params[x].height) {
              if ((params[x].amount + params[x].inc) > 255) {
                params[x].amount = 0;
                params[x].height++;
              }
              else {
                params[x].amount += params[x].inc;
              }
            }
            count++;
          }
          for (uint8_t i = height; i > 0; i--) {
            uint8_t y = i - 1;
            if (y > params[x].height) {
              if (y == params[x].y) {
                setHsvXY(x, y, params[x].hue, 255, params[x].inc);
              }
            } else if (y == params[x].height) {
              setHsvXY(x, y, params[x].hue, 255, params[x].amount);
            } else {
              setHueXY(x, y, params[x].hue);
            }
          }
        }
      }
      return count > 0;
    }

    bool WindowLed::effectFade(bool firstStart) {
      static FADE_PARAMS params[2];
      const uint8_t step = 5;
      const uint8_t huestep = 15;
      const uint8_t limit = 180;
      bool retVal = true;
      if (firstStart) {
        params[0].value = 0;
        params[0].hue = 0;
        params[0].fade = false;
        params[0].skip = false;
        params[1].value = 0;
        params[1].hue = huestep;
        params[1].fade = false;
        params[1].skip = true;
        FastLED.clearData();
        Serial.println(__FUNCTION__);
      }
      for (uint8_t i = 0; i < 2; i++) {
        if (params[i].skip) continue;
        if (params[i].fade) {
          if ((params[i].value >= limit) && (params[i].value - step < limit)) {
            params[i ^ 1].skip = false;
            params[i ^ 1].fade = false;
            params[i ^ 1].value = 0;
          }
          if (params[i].value - step <= 0) {
            params[i].value = 0;
            params[i].fade = false;
            if (params[i].hue + 2 * huestep > 255)
              retVal = false;
            params[i].hue += 2 * huestep;
            params[i].skip = true;
          }
          else {
            params[i].value -= step;
          }
        } else {
          if (params[i].value + step >= 255) {
            params[i].value = 255;
            params[i].fade = true;
          }
          else {
            params[i].value += step;
          }
        }
        /*
        Serial.print("i = ");
        Serial.print(i);
        Serial.print(" fade = ");
        Serial.print(params[i].fade);
        Serial.print(" hue = ");
        Serial.print(params[i].hue);
        Serial.print(" value = ");
        Serial.println(params[i].value);
        */
      }
      for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
          setHsvXY(x, y, params[(x + (y & 1)) & 1].hue, 255, params[(x + (y & 1)) & 1].value);
        }
      }
      return retVal;
    }

    bool WindowLed::effectFill(bool firstStart) {
      static uint8_t hue = 0;
      static int x = 0;
      static int y = 0;
      static int dx = 0;
      static int dy = 0;
      static uint8_t direction = 0;
      static uint8_t start_point = 0;
      static int8_t changeColor = 0;
      if (firstStart) {
        hue = random(256);
        direction = ((random(256) & 1) == 0) ? LINES_HORIZONTAL : LINES_VERTICAL;
        changeColor = ((random(256) % 3) - 1) * 30;
        start_point = random(256) % (START_AT_BOTTOMRIGHT + 1);
        if (start_point == START_AT_BOTTOMLEFT) {
          x = 0;
          y = 0;
          dx = 1;
          dy = 1;
        }
        else if (start_point == START_AT_TOPLEFT) {
          x = 0;
          y = height - 1;
          dx = 1;
          dy = -1;
        }
        else if (start_point == START_AT_BOTTOMRIGHT) {
          x = width - 1;
          y = 0;
          dx = -1;
          dy = 1;
        }
        else if (start_point == START_AT_TOPRIGHT) {
          x = width - 1;
          y = height - 1;
          dx = -1;
          dy = -1;
        }
        Serial.print(__FUNCTION__);
        Serial.print(" ");
        Serial.print(direction == LINES_HORIZONTAL ? "HORIZONTAL" : "VERTICAL");
        Serial.print(" ");
        Serial.print(start_point == START_AT_TOPLEFT ? "TOPLEFT" : start_point == START_AT_TOPRIGHT ? "TOPRIGHT" : start_point == START_AT_BOTTOMLEFT ? "BOTTOMLEFT" : "BOTTOMRIGHT");
        Serial.print(" ");
        Serial.print(changeColor);
        Serial.print(" ");
        Serial.println(hue);
        FastLED.clearData();
      }
      bool retVal = true;
      setHueXY(x, y, hue);
      if (direction == LINES_HORIZONTAL) {
        if (
          ((dy > 0) && ((y & 1) == 0))
          || ((dy < 0) && (((height - 1 - y) & 1) == 0))
        ) {
          x += dx;
          if (
            ((dx > 0) && (x >= width))
            || ((dx < 0) && (x < 0))
          ) {
            x = (dx > 0) ? width - 1 : 0;
            y += dy;
            hue += changeColor;
          }
        } else {
          x -= dx;
          if (
            ((dx > 0) && (x < 0))
            || ((dx < 0) && (x >= width))
          ) {
            x = (dx > 0) ? 0 : width - 1;
            y += dy;
            hue += changeColor;
          }
        }
        retVal = (dy > 0) ? (y < height) : (y >= 0);
      } else {
        if (
          ((dx > 0) && ((x & 1) == 0))
          || ((dx < 0) && (((width - 1 - x) & 1) == 0))
        ) {
          y += dy;
          if (
            ((dy > 0) && (y >= height))
            || ((dy < 0) && (y < 0))
          ) {
            y = (dy > 0) ? height - 1 : 0;
            x += dx;
            hue += changeColor;
          }
        } else {
          y -= dy;
          if (
            ((dy > 0) && (y < 0))
            || ((dy < 0) && (y >= height))
          ) {
            y = (dy > 0) ? 0 : height - 1;
            x += dx;
            hue += changeColor;
          }
        }
        retVal = (dx > 0) ? (x < width) : (x >= 0);
      }
      return retVal;
    }

    bool WindowLed::effectTowards(bool firstStart) {
      static uint8_t hue = 0;
      static uint8_t shift = 0;
      static uint8_t direction = 0;
      static uint8_t waitCounter = 0;
      if (firstStart) {
        hue = random(256);
        direction = ((hue & 1) == 0) ? LINES_HORIZONTAL : LINES_VERTICAL;
        shift = 0;
        Serial.print(__FUNCTION__);
        Serial.print(" ");
        Serial.print(direction == LINES_HORIZONTAL ? "HORIZONTAL" : "VERTICAL");
        Serial.print(" ");
        Serial.println(hue);
        FastLED.clearData();
      }
      if (waitCounter-- > 0) {
        return true;
      }
      if (direction == LINES_HORIZONTAL) {
        if (shift < width) {
          for (uint8_t y = 0; y < height; y++) {
            setHueXY(((y & 1) == 0) ? shift : width - shift - 1, y, hue);
          }
        } else {
          for (uint8_t y = 0; y < height; y++) {
            setRgbXY(((y & 1) == 0) ? shift - width : width + width - shift - 1, y, 0, 0, 0);
          }
        }
      } else {
        if (shift < height) {
          for (uint8_t x = 0; x < width; x++) {
            setHueXY(x, ((x & 1) == 0) ? shift : height - shift - 1, hue);
          }
        } else {
          for (uint8_t x = 0; x < width; x++) {
            setRgbXY(x, ((x & 1) == 0) ? shift - height : height + height - shift - 1, 0, 0, 0);
          }
        }
      }
      shift++;
      waitCounter = 2;
      bool retVal = (shift < ((direction == LINES_HORIZONTAL) ? width : height) * 2);
      return retVal;
    }

    bool WindowLed::effectSquareSnake(bool firstStart, bool changeColor) {
      static uint8_t x = 0;
      static uint8_t y = 0;
      static uint8_t l = 0;
      static int8_t dx = 1;
      static int8_t dy = 0;
      static int8_t dh = 0;
      static int8_t w = width;
      static int8_t h = height;
      static uint8_t hue = 0;
      if (firstStart) {
        w = width;
        h = height;
        x = 0;
        y = 0;
        dx = 1;
        dy = 0;
        l = w - 1;
        hue = random(256);
        dh = 50 * (((hue & 1) == 0) ? 1 : -1);
        FastLED.clearData();
        // Serial.println();
        Serial.print(__FUNCTION__);
        Serial.print(" ");
        Serial.print(changeColor ? dh : 0);
        Serial.println(changeColor ? " ChangeColor" : " Mono");
      }
      setHueXY(x, y, hue);
      /* Serial.print("(");
      Serial.print(x);
      Serial.print(",");
      Serial.print(y);
      Serial.print(") ");*/
      if (dx > 0) {
        if (l == 0) {
          // Serial.println();
          dx = 0;
          dy = 1;
          y++;
          l = --h;
        }
        else {
          x++;
        }
      }
      else if (dy > 0) {
        if (l == 0) {
          // Serial.println();
          dx = -1;
          dy = 0;
          x--;
          l = --w;
        }
        else {
          y++;
        }
      }
      else if (dx < 0) {
        if (l == 0) {
          // Serial.println();
          dx = 0;
          dy = -1;
          y--;
          l = --h;
        }
        else {
          x--;
        }
      }
      else if (dy < 0) {
        if (l == 0) {
          // Serial.println();
          dx = 1;
          dy = 0;
          x++;
          l = --w;
          if (changeColor) {
            hue += 50;
          }
        }
        else {
          y--;
        }
      }
      l--;
      return w > 0 && h > 0;
    }

    bool WindowLed::effectSquareSnake(bool firstStart) {
      static bool changeColor = true;
      if (firstStart) {
        int r = random(250);
        changeColor = ((r & 1) == 1);
      }
      return this->effectSquareSnake(firstStart, changeColor);
    }

    bool WindowLed::effectInit(bool firstStart) {
      static uint8_t step = 0;
      static uint8_t x = 0;
      static uint8_t y = 0;
      static uint8_t waitCounter = 0;
      if (!firstStart) {
        if ((step < 4) && (waitCounter-- > 0)) {
          return true;
        } else {
          if (step < 4) {
            step++;
          }
        }
      }
      if (step == 0) {
        this->fillRgb(255, 0, 0);
        Serial.println("Red");
        waitCounter = WINDOWLED_TEST_TIME;
        return true;
      }
      else if (step == 1) {
        this->fillRgb(0, 255, 0);
        Serial.println();
        Serial.println("Green");
        waitCounter = WINDOWLED_TEST_TIME;
        return true;
      }
      else if (step == 2) {
        this->fillRgb(0, 0, 255);
        Serial.println();
        Serial.println("Blue");
        waitCounter = WINDOWLED_TEST_TIME;
        return true;
      }
      else if (step == 3) {
        this->fillRgb(255, 255, 255);
        Serial.println();
        Serial.println("White");
        waitCounter = WINDOWLED_TEST_TIME;
        return true;
      }
      else if (step == 4) {
        if ((x == 0) && (y == 0)) {
          FastLED.clearData();
          Serial.println();
          Serial.println("Random fill");
        }
        if (x >= this->width) {
          Serial.println();
          y++;
          x = 0;
        }
        if (y >= this->height) {
          return false;
        }
        setHueXY(x++, y, random(256));
        return true;
      }
      return false;
    }

    uint8_t WindowLed::getEffectTimes(unsigned char index) {
      /*
      Serial.print("getEffectTimes index = ");
      Serial.print(index);
      Serial.print(" times limit = ");
      Serial.println(effectList[index].times);
      */
      return effectList[index].times > 0 ? effectList[index].times : (random(-effectList[index].times) + 1);
    }

    void WindowLed::showCurrentSequence(unsigned long now) {
      Serial.println();
      Serial.print(now);
      Serial.print(" Seq = ");
      Serial.print(currentEffect);
      Serial.print(" t = ");
      Serial.print(currentTime);
      Serial.println();
    }

    void WindowLed::update(unsigned long now) {
      static bool firstStart = true;
      if (this->isOn) {
        switch (effectList[currentEffect].funcNumber) {
          case WINDOWLED_INIT:
            firstStart = !this->effectInit(firstStart);
            break;
          case WINDOWLED_RAINBOW:
            firstStart = !this->effectRainbow(firstStart);
            break;
          case WINDOWLED_FILL:
            firstStart = !this->effectFill(firstStart);
            break;
          case WINDOWLED_TOWARDS:
            firstStart = !this->effectTowards(firstStart);
            break;
          case WINDOWLED_SQARESNAKE:
            firstStart = !this->effectSquareSnake(firstStart);
            break;
          case WINDOWLED_FADE:
            firstStart = !this->effectFade(firstStart);
            break;
          case WINDOWLED_DROP:
            firstStart = !this->effectDrop(firstStart);
            break;
        }
        FastLED.show(100);
        Serial.print(".");
        if (firstStart) {
          if (--currentTime <= 0) {
            currentEffect++;
            if (currentEffect >= this->effectCount) {
              currentEffect = 1;
            }
            currentTime = getEffectTimes(currentEffect);
          }
          showCurrentSequence(now);
        }
      }
    }

    void WindowLed::on() {
      this->isOn = true;
    }

    void WindowLed::off() {
      this->isOn = false;
      FastLED.show(0);
    }

    bool WindowLed::getState() {
      return this->isOn;
    }
  }
}
