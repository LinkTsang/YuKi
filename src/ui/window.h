#ifndef YUKI_WINDOW_H_
#define YUKI_WINDOW_H_

#include <memory>
#include "core/string.hpp"
#include "ui/view.h"

namespace yuki {
enum class WindowState { Shown, Hidden, Max, Min };

class INativeWindow {
 public:
  virtual void setView(const std::shared_ptr<View>& view) = 0;

  virtual void setTitle(const String& title) = 0;
  virtual String getTitle() const = 0;

  virtual void show() = 0;
  virtual void hide() = 0;
  virtual void setWindowState(WindowState state) = 0;

  virtual void moveToCenter() = 0;
  virtual void moveTo(const Point& p) = 0;

  virtual ~INativeWindow();
};

class Window {
 public:
  Window();
  explicit Window(const std::shared_ptr<View>& view);
  void show() { nativeWindow_->show(); }
  void hide() { nativeWindow_->hide(); }
  void setTitle(const String& title) { nativeWindow_->setTitle(title); }
  void setView(const std::shared_ptr<View>& view) {
    nativeWindow_->setView(view);
  }
  void moveToCenter() { nativeWindow_->moveToCenter(); }
  void moveTo(const Point& p) { nativeWindow_->moveTo(p); }
  void moveTo(int x, int y) { nativeWindow_->moveTo(Point{x, y}); }

 private:
  std::unique_ptr<INativeWindow> nativeWindow_;
};
}  // namespace yuki

#endif  // !YUKI_WINDOW_H_