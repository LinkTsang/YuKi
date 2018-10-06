#pragma once

#include <memory>
#include "core/string.hpp"
#include "ui/view.h"
#include "ui/window.h"

#ifdef TEXT
#undef TEXT
#endif
#include <Windows.h>

namespace yuki {
class D2DContext2D;

class NativeWindowManager {
public:
  static const TCHAR WINDOW_CLASS_NAME[];
  static void init();
  static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
};

class NativeWindowImpl : public INativeWindow {
public:
  explicit NativeWindowImpl(Window* window, std::shared_ptr<View> view);

  WindowState getWindowState() override;
  void setWindowState(WindowState state) override;

  void setTitle(const String& title) override;
  String getTitle() const override;

  Rect getBounds() const override;
  void setBounds(const Rect& bounds) override;

  void setView(std::shared_ptr<View> view) override;
  std::shared_ptr<View> getView() const override;
private:
  static const TCHAR DEFAULT_WINDOW_TITLE[];
  friend class NativeWindowManager;
  HWND hWnd_;
  Window* const window_;
  std::shared_ptr<View> view_;
  std::unique_ptr<D2DContext2D> context_;
};
} // namespace yuki
