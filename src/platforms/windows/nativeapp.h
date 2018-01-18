#pragma once
#include <Windows.h>

namespace yuki {
  class NativeApp {
  public:
    static void init();
    static HINSTANCE getInstance();
    static int messageLoop();
    static int run();
  private:
    static void terminate();
  };
};