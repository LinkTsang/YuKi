#include "window.h"

#include "platforms/windows/window.h"

namespace yuki {
INativeWindow::~INativeWindow() = default;

Window::Window() : Window(std::make_shared<View>()) {}

Window::Window(const std::shared_ptr<View>& view) : nativeWindow_(std::make_unique<NativeWindowImpl>(view)) {}
}  // namespace yuki