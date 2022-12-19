#ifndef WINDOWLED_SETTINGS
#define WINDOWLED_SETTINGS

namespace Nezumikun {
  namespace WindowLed {

    enum class DebugLevel {
      None,
      Debug,
      Verbose
    };

    class Settings {
      public:
        static DebugLevel debugLevel;
    };

  }
}

#endif
