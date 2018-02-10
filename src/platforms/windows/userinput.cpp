#include "userinput.h"
#include <Windows.h>

namespace yuki {
WinKeyEventArgs::WinKeyEventArgs(const Key key) : KeyEventArgs(key) {}

bool WinKeyEventArgs::isAltPressed() const {
  return GetKeyState(VK_MENU) & 0x8000;
}

bool WinKeyEventArgs::isCtrlPressed() const {
  return GetKeyState(VK_CONTROL) & 0x8000;
}

bool WinKeyEventArgs::isShiftPressed() const {
  return GetKeyState(VK_SHIFT) & 0x8000;
}

bool WinKeyEventArgs::isKeyPressed(Key key) const {
  return GetKeyState(static_cast<int>(key)) & 0x8000;
}
}
