#pragma once

#include "ui/userinput.h"

namespace yuki {
class WinKeyEventArgs : public KeyEventArgs {
public:
  explicit WinKeyEventArgs(Key key);
  bool isAltPressed() const override;
  bool isCtrlPressed() const override;
  bool isShiftPressed() const override;
  bool isKeyPressed(Key key) const override;
};
}
