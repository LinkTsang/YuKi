#pragma once

#include <memory>

#ifdef TEXT
#undef TEXT
#endif
#include <Windows.h>

#include "platforms/windows/direct2d.h"
#include "ui/window.h"

namespace yuki {
namespace platforms {
namespace windows {

class D2DContext2D;

class NativeWindowManager {
 public:
  static const TCHAR WINDOW_CLASS_NAME[];
  static void init();
  static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
};

class NativeWindowImpl : public yuki::ui::INativeWindow {
 public:
  explicit NativeWindowImpl(yuki::ui::Window* window,
                            std::shared_ptr<yuki::ui::View> view);

  yuki::ui::WindowState getWindowState() override;
  void setWindowState(yuki::ui::WindowState state) override;

  void setTitle(const String& title) override;
  String getTitle() const override;

  yuki::graphic::Rect getBounds() const override;
  void setBounds(const yuki::graphic::Rect& bounds) override;

  void setView(std::shared_ptr<yuki::ui::View> view) override;
  std::shared_ptr<yuki::ui::View> getView() const override;

 private:
  static const TCHAR DEFAULT_WINDOW_TITLE[];
  friend class NativeWindowManager;
  HWND hWnd_;
  yuki::ui::Window* const window_;
  std::shared_ptr<yuki::ui::View> view_;
  std::unique_ptr<D2DContext2D> context_;
};
}  // namespace windows
}  // namespace platforms
}  // namespace yuki
