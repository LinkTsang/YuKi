#pragma once

#include "ui/userinput.h"

namespace yuki {
namespace platforms {
namespace windows {
using namespace ui;
class WinKeyEventArgs : public KeyEventArgs {
 public:
  explicit WinKeyEventArgs(Key key);
  bool isAltPressed() const override;
  bool isCtrlPressed() const override;
  bool isShiftPressed() const override;
  bool isKeyPressed(Key key) const override;
};
}  // namespace windows
}  // namespace platforms
}  // namespace yuki