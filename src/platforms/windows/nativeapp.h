#pragma once
#include <Windows.h>

namespace yuki {
namespace platforms {
namespace windows {
class NativeApp {
 public:
  static void init();
  static HINSTANCE getInstance();
  static int messageLoop();
  static int run();

 private:
  static void terminate();
};
}  // namespace windows
}  // namespace platforms
}  // namespace yuki