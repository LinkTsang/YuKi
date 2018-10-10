#pragma once

#include <memory>
#include "core/object.h"
#include "core/string.hpp"
#include "ui/userinput.h"
#include "ui/view.h"

namespace yuki {
namespace ui {
class View;

class INativeWindow : public Object {
 public:
  INativeWindow() = default;
  INativeWindow(const INativeWindow&) = default;
  INativeWindow(INativeWindow&&) = default;
  INativeWindow& operator=(const INativeWindow&) = default;
  INativeWindow& operator=(INativeWindow&&) = default;
  virtual ~INativeWindow() = default;

  virtual void setView(std::shared_ptr<View> view) = 0;
  virtual std::shared_ptr<View> getView() const = 0;

  virtual String getTitle() const = 0;
  virtual void setTitle(const String& title) = 0;

  virtual Rect getBounds() const = 0;
  virtual void setBounds(const Rect& bounds) = 0;

  virtual WindowState getWindowState() = 0;
  virtual void setWindowState(WindowState state) = 0;
};

class Window : public Object {
 public:
  Window();
  explicit Window(std::shared_ptr<View> view);
  Window(const Window&) = delete;
  Window(Window&&) = default;
  Window& operator=(const Window&) = delete;
  Window& operator=(Window&&) = default;
  virtual ~Window() = default;

  void setView(std::shared_ptr<View> view) { w_->setView(std::move(view)); }
  std::shared_ptr<View> getView() const { return w_->getView(); }

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

  virtual void activateEvent(ActivateEventArgs* args);
  virtual void closingEvent(ClosingEventArgs* closed);
  virtual void closedEvent();
  virtual void windowStateChangeEvent(WindowStateChangedEventArgs* args);
  virtual void movingEvent(WindowMovingEventArgs* args);
  virtual void movedEvent(WindowMovedEventArgs* args);

 protected:
  std::unique_ptr<INativeWindow> w_;
};

}  // namespace ui
}  // namespace yuki