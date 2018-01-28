#include "app.h"
#include "platforms/windows/nativeapp.h"

namespace yuki {
void App::init() { NativeApp::init(); }
int App::run() { return NativeApp::run(); }
}; // namespace yuki
