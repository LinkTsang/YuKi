#include "ui/view.h"

namespace yuki {
const UIContainer& View::children() const { return children_; }
UIContainer& View::children() { return children_; }

void View::onRenderTargetChanged(Context2D* context) {
  children_.onRenderTargetChanged(context);
}

void View::onRender(Context2D* context) {
  context->clear(Color::White);
  children_.onRender(context);
}

void View::sizeChangedEvent(SizeChangedEventArgs* args) {
  getBounds().setSize(args->getSize());
}

void View::sizeChangingEvent(SizeChangingEventArgs* args) {}
void View::mouseButtonUpEvent(MouseEventArgs* args) {}
void View::mouseButtonDownEvent(MouseEventArgs* args) {}
void View::mouseButtonDoubleClickEvent(MouseEventArgs* args) {}
void View::mouseMoveEvent(MouseEventArgs* args) {}
void View::mouseWheelEvent(MouseEventArgs* args) {}
void View::keyDownEvent(KeyEventArgs* args) {}
void View::keyCharEvent(KeyCharEventArgs* args) {}
void View::keyUpEvent(KeyEventArgs* args) {}
} // namespace yuki
