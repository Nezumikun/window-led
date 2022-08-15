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

  char * Uptime::toString() {
    if (this->info.days > 0) {
      sprintf(buffer, "%dd ", this->info.days);
    }
    sprintf(buffer + strlen(buffer), "%02d:%02d:%02d", this->info.hours, this->info.minutes, this->info.seconds);
    return buffer;
  }

}