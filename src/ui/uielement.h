#pragma once
#include <memory>
#include "graphics/painter.h"
namespace yuki {
class UIElement {
 public:
  virtual ~UIElement() = default;
 protected:
  virtual void onRender(Context2D* context);
  friend class NativeWindowManager;
};
}