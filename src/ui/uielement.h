#pragma once
#include "core/object.h"
#include "graphics/painter.h"

namespace yuki {
class UIElement : public Object {
public:
  UIElement() = default;
  UIElement(const UIElement&) = default;
  UIElement(UIElement&&) = default;
  UIElement& operator=(const UIElement&) = default;
  UIElement& operator=(UIElement&&) = default;
  virtual ~UIElement() = default;

protected:
  virtual void onRender(Context2D* context);
  friend class NativeWindowManager;
};
} // namespace yuki
