#pragma once

#include <memory>
#include "core/string.hpp"
#include "ui/view.h"

namespace yuki {

enum class WindowState { Shown, Hidden, Restore, Maximized, Minimized };

class INativeWindow {
public:
  INativeWindow() = default;
  INativeWindow(const INativeWindow&) = default;
  INativeWindow(INativeWindow&&) = default;
  INativeWindow& operator=(const INativeWindow&) = default;
  INativeWindow& operator=(INativeWindow&&) = default;
  virtual ~INativeWindow() = default;

  virtual void setView(const std::shared_ptr<View>& view) = 0;

  virtual String getTitle() const = 0;
  virtual void setTitle(const String& title) = 0;

  virtual Rect getBounds() const = 0;
  virtual void setBounds(const Rect& bounds) = 0;

  virtual WindowState getWindowState() = 0;
  virtual void setWindowState(WindowState state) = 0;
};

class Window : Object {
public:
  Window();
  explicit Window(const std::shared_ptr<View>& view);
  Window(const Window&) = delete;
  Window(Window&&) = default;
  Window& operator=(const Window&) = delete;
  Window& operator=(Window&&) = default;
  virtual ~Window() = default;

  void setView(const std::shared_ptr<View>& view) { w_->setView(view); }

  void show() { w_->setWindowState(WindowState::Shown); }
  void hide() { w_->setWindowState(WindowState::Hidden); }
  void restore() { w_->setWindowState(WindowState::Restore); }
  void maximize() { w_->setWindowState(WindowState::Maximized); }
  void minimize() { w_->setWindowState(WindowState::Minimized); }
  WindowState getWindowState() { return w_->getWindowState(); }
  void setWindowState(WindowState state) { w_->setWindowState(state); }

  void setTitle(const String& title) { w_->setTitle(title); }
  String getTitle() const { return w_->getTitle(); }

  Rect getBounds() const { return w_->getBounds(); }
  void setBounds(const Rect& bounds) { w_->setBounds(bounds); }

private:
  std::unique_ptr<INativeWindow> w_;
};
} // namespace yuki
