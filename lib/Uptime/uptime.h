#ifndef NEZUMIKUN_UPTIME
#define NEZUMIKUN_UPTIME

#include <Arduino.h>

namespace Nezumikun {
  class Uptime {
    private:
      unsigned long prevMillis;
    public:
      Uptime();
      void reset();
      void loop();
      String toString();
      struct Info {
        unsigned long days;
        unsigned char hours;
        unsigned char minutes;
        unsigned char seconds;
      } info;
  };
}

#endif