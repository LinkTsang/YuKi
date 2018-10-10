#include "app.h"

#ifdef _WIN32
#include "platforms/windows/nativeapp.h"
using namespace yuki::platforms::windows;
#else
#error Not supported platform
#endif

namespace yuki {
void App::init() { NativeApp::init(); }
int App::run() { return NativeApp::run(); }
};  // namespace yuki
