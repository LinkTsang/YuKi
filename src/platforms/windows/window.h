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
  void setView(const std::shared_ptr<View>& view) override;

  WindowState getWindowState() override;
  void setWindowState(WindowState state) override;

  void setTitle(const String& title) override;
  String getTitle() const override;

  Rect getBounds() const override;
  void setBounds(const Rect& bounds) override;
  
 private:
  static const TCHAR DEFAULT_WINDOW_TITLE[];
  friend class NativeWindowManager;
  HWND hWnd_;
  std::shared_ptr<View> view_;
  std::unique_ptr<D2DContext2D> context_;
};

}  // namespace yuki