#pragma once
#include "uielement.h"
#include "ui/userinput.h"

namespace yuki {
class View : public UIElement {
public:
  View() = default;
  View(const View&) = default;
  View(View&&) = default;
  View& operator=(const View&) = default;
  View& operator=(View&&) = default;
  virtual ~View() = default;

protected:
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
};
} // namespace yuki
