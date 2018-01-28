#include "nativeapp.h"
#include "platforms/windows/direct2d.h"
#include "platforms/windows/window.h"
#include <Windows.h>

namespace yuki {
/*******************************************************************************
* class NativeApp
******************************************************************************/

void NativeApp::init() {
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
}
