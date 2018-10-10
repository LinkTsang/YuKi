#include "nativeapp.h"
#include <Windows.h>
#include "core/logger.h"
#include "platforms/windows/direct2d.h"
#include "platforms/windows/window_impl.h"

namespace yuki {
namespace platforms {
namespace windows {
/*******************************************************************************
 * class Win32LoggerListener
 ******************************************************************************/
class Win32LoggerListener : public ILoggerListener {
 public:
  Win32LoggerListener() = default;
  Win32LoggerListener(const Win32LoggerListener&) = default;
  Win32LoggerListener(Win32LoggerListener&&) = default;
  Win32LoggerListener& operator=(const Win32LoggerListener&) = default;
  Win32LoggerListener& operator=(Win32LoggerListener&&) = default;
  virtual ~Win32LoggerListener() = default;

  void write(const String& message) override {
    ::OutputDebugString(std::wstring(message.cbegin(), message.cend()).c_str());
  }
};

/*******************************************************************************
 * class NativeApp
 ******************************************************************************/

void NativeApp::init() {
  Logger::addListener(std::make_shared<Win32LoggerListener>());

  NativeWindowManager::init();
  DirectXRes::init();
}

HINSTANCE NativeApp::getInstance() { return ::GetModuleHandle(nullptr); }

inline int NativeApp::messageLoop() {
  MSG msg;
  BOOL result;
  while (result = ::GetMessage(&msg, nullptr, 0, 0)) {
    if (!::TranslateAccelerator(msg.hwnd, nullptr, &msg)) {
      TranslateMessage(&msg);
      ::DispatchMessage(&msg);
    } else if (result == -1) {
      // TODO: handle the error and possibly exit
      throw;
    }
  }
  return static_cast<int>(msg.wParam);
}

int NativeApp::run() {
  int result = messageLoop();
  terminate();
  return result;
}

void NativeApp::terminate() { DirectXRes::releaseAll(); }
}  // namespace windows
}  // namespace platforms
}  // namespace yuki