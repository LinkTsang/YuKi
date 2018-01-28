#pragma once
#include "uielement.h"

namespace yuki {
class View : public UIElement {
public:
  View() = default;
  View(const View&) = default;
  View(View&&) = default;
  View& operator=(const View&) = default;
  View& operator=(View&&) = default;
  virtual ~View() = default;
};
} // namespace yuki
