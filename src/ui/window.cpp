#include "window.h"
#include "platforms/windows/window.h"

namespace yuki {
Window::Window() : Window(std::make_shared<View>()) { }

Window::Window(std::shared_ptr<View> view)
  : w_(std::make_unique<NativeWindowImpl>(this, std::move(view))) { }

void Window::activateEvent(ActivateEventArgs* args) {}
void Window::closingEvent(ClosingEventArgs* args) {}
void Window::closedEvent() {}
void Window::windowStateChangeEvent(WindowStateChangedEventArgs* args) {}

} // namespace yuki
