#include "uptime.h"
#include <Arduino.h>

namespace Nezumikun {

  Uptime::Uptime() {
    this->reset();
  }

  void Uptime::reset() {
    this->info.days = 0;
    this->info.hours = 0;
    this->info.minutes = 0;
    this->info.seconds = 0;
    this->prevMillis = millis() / 1000 * 1000;
  }

  void Uptime::loop() {
    unsigned long now = millis();
    if (now - this->prevMillis > 1000) {
      unsigned char seconds = (now - this->prevMillis) / 1000;
      this->info.seconds += seconds;
      if (this->info.seconds > 59) {
        this->info.seconds = 0;
        this->info.minutes++;
      }
      if (this->info.minutes > 59) {
        this->info.minutes = 0;
        this->info.hours++;
      }
      if (this->info.hours > 23) {
        this->info.hours = 0;
        this->info.days++;
      }
      this->prevMillis = now / 1000 * 1000;
    }
  }

  String Uptime::toString() {
    String res = String("");
    if (this->info.days > 0) {
      res += String(this->info.days) + " d. ";
    }
    res += (this->info.hours < 10 ? "0" : "") +  String(this->info.hours) + ":";
    res += (this->info.minutes < 10 ? "0" : "") +  String(this->info.minutes) + ":";
    res += (this->info.seconds < 10 ? "0" : "") +  String(this->info.seconds);
    return res;
  }

}