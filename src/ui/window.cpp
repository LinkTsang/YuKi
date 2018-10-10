#include "window.h"
#include "platforms/windows/window_impl.h"

namespace yuki {
namespace ui {

#ifdef _WIN32
using namespace yuki::platforms::windows;
#else
#error Not supported platform
#endif

Window::Window() : Window(std::make_shared<View>()) {}

Window::Window(std::shared_ptr<View> view)
    : w_(std::make_unique<NativeWindowImpl>(this, std::move(view))) {}

void Window::activateEvent(ActivateEventArgs* args) {}
void Window::closingEvent(ClosingEventArgs* args) {}
void Window::closedEvent() {}
void Window::windowStateChangeEvent(WindowStateChangedEventArgs* args) {}
void Window::movingEvent(WindowMovingEventArgs* args) {}
void Window::movedEvent(WindowMovedEventArgs* args) {}
}  // namespace ui
}  // namespace yuki