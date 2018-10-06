#pragma once
#include "uielement.h"
#include "ui/userinput.h"
#include <vector>

namespace yuki {
class View : public UIElement {
public:
  View() = default;
  View(const View&) = default;
  View(View&&) = default;
  View& operator=(const View&) = default;
  View& operator=(View&&) = default;
  virtual ~View() = default;

  const UIContainer& children() const;
  UIContainer& children();
protected:
  void onRenderTargetChanged(Context2D* context) override;
  void onRender(Context2D* context) override;
  virtual void sizeChangedEvent(SizeChangedEventArgs* args);
  virtual void sizeChangingEvent(SizeChangingEventArgs* args);
  virtual void mouseButtonUpEvent(MouseEventArgs* args);
  virtual void mouseButtonDownEvent(MouseEventArgs* args);
  virtual void mouseButtonDoubleClickEvent(MouseEventArgs* args);
  virtual void mouseMoveEvent(MouseEventArgs* args);
  virtual void mouseWheelEvent(MouseEventArgs* args);
  virtual void keyDownEvent(KeyEventArgs* args);
  virtual void keyCharEvent(KeyCharEventArgs* args);
  virtual void keyUpEvent(KeyEventArgs* args);
  friend class NativeWindowManager;

private:
  UIContainer children_;
};
} // namespace yuki
