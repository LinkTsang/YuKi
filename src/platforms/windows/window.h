#pragma once

#include <Windows.h>
#include <memory>
#include "core/string.hpp"
#include "ui/view.h"
#include "ui/window.h"

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
  NativeWindowImpl();
  explicit NativeWindowImpl(const std::shared_ptr<View>& view);
  void show() override;
  void hide() override;
  void setTitle(const String& title) override;
  void setView(const std::shared_ptr<View>& view) override;

  String getTitle() const override;
  void moveToCenter() override;
  void moveTo(const Point& p) override;
  void setWindowState(WindowState state) override;
  Size getSize() override;
 private:
  static const TCHAR DEFAULT_WINDOW_TITLE[];
  friend class NativeWindowManager;
  HWND hWnd_;
  std::shared_ptr<View> view_;
  std::unique_ptr<D2DContext2D> context_;
};

}  // namespace yuki