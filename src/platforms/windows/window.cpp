#include "window.h"
#include <Windows.h>
#include <Windowsx.h>
#include "graphics/painter.h"
#include "platforms/windows/direct2d.h"
#include "platforms/windows/nativeapp.h"

namespace yuki {
class EventArgs {
 public:
  EventArgs() = default;
};

enum class MouseKeyStateMask {
  LButton = 0x0001,
  RButton = 0x0002,
  Shift = 0x0004,
  Control = 0x0008,
  MButton = 0x0010,
  XButton1 = 0x0020,
  XButton2 = 0x0040
};

class MouseEventArgs : public EventArgs {
 public:
  MouseEventArgs(Point position, int mouseKeyState, int delta)
      : position_(position), mouseKeyState_(mouseKeyState), delta_(delta){};
  const Point& position() const { return position_; }
  int delta() const { return delta_; }
  bool isLButtonDown() const {
    return mouseKeyState_ & static_cast<int>(MouseKeyStateMask::LButton);
  }
  bool isRButtonDown() const {
    return mouseKeyState_ & static_cast<int>(MouseKeyStateMask::RButton);
  }
  bool isShiftDown() const {
    return mouseKeyState_ & static_cast<int>(MouseKeyStateMask::Shift);
  }
  bool isControlDown() const {
    return mouseKeyState_ & static_cast<int>(MouseKeyStateMask::Control);
  }
  bool isMButtonDown() const {
    return mouseKeyState_ & static_cast<int>(MouseKeyStateMask::MButton);
  }
  bool isXButton1Down() const {
    return mouseKeyState_ & static_cast<int>(MouseKeyStateMask::XButton1);
  }
  bool isXButton2Down() const {
    return mouseKeyState_ & static_cast<int>(MouseKeyStateMask::XButton2);
  }

 private:
  Point position_;
  int mouseKeyState_;
  int delta_;
};
class KeyEventArgs : public EventArgs {
 public:
  KeyEventArgs() = default;
};
/*******************************************************************************
 * class NativeWindowManager
 ******************************************************************************/

const TCHAR NativeWindowManager::WINDOW_CLASS_NAME[] =
    TEXT("YUKI_WINDOW_CLASS");

void NativeWindowManager::init() {
  WNDCLASSEX wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = NativeApp::getInstance();
  wcex.hIcon = nullptr;  // LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DEMO));
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = nullptr;  // MAKEINTRESOURCEW(IDC_DEMO);
  wcex.lpszClassName = WINDOW_CLASS_NAME;
  wcex.hIconSm =
      nullptr;  // LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

  RegisterClassEx(&wcex);
}

LRESULT NativeWindowManager::WndProc(HWND hWnd, UINT message, WPARAM wParam,
                                     LPARAM lParam) {
  NativeWindowImpl* w =
      reinterpret_cast<NativeWindowImpl*>(GetWindowLong(hWnd, GWL_USERDATA));

  if (WM_MOUSEFIRST <= message && message <= WM_MOUSELAST) {
    MouseEventArgs event_args{{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)},
                              GET_KEYSTATE_WPARAM(wParam),
                              GET_WHEEL_DELTA_WPARAM(wParam)};
    switch (message) {
      case WM_MOUSEMOVE:
        break;
      case WM_LBUTTONDOWN:
      case WM_RBUTTONDOWN:
      case WM_MBUTTONDOWN:
        break;
      case WM_LBUTTONUP:
      case WM_RBUTTONUP:
      case WM_MBUTTONUP:
        break;
      case WM_LBUTTONDBLCLK:
      case WM_RBUTTONDBLCLK:
      case WM_MBUTTONDBLCLK:
        break;
      case WM_MOUSEWHEEL:
        break;
      default:
        break;
    }
  }
  switch (message) {
    case WM_PAINT: {
      PAINTSTRUCT ps;
      ::BeginPaint(hWnd, &ps);
      w->context_->beginDraw();
      w->view_->onRender(w->context_.get());
      ::EndPaint(hWnd, &ps);
      w->context_->endDraw();
    } break;
    case WM_SIZE: {
      RECT rect;
      GetClientRect(hWnd, &rect);
      SizeF size({static_cast<float>(rect.right - rect.left),
                  static_cast<float>(rect.bottom - rect.top)});

      w->context_->resetSize(size);
    } break;
    case WM_SIZING:
      break;
    case WM_CLOSE: {
      bool isClosed = true;
      if (isClosed) DestroyWindow(hWnd);
    } break;
    case WM_DESTROY: {
      PostQuitMessage(0);
    } break;
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

/*******************************************************************************
 * class NativeWindowImpl
 ******************************************************************************/

const TCHAR NativeWindowImpl::DEFAULT_WINDOW_TITLE[] = TEXT("Test");

NativeWindowImpl::NativeWindowImpl()
    : NativeWindowImpl(std::make_shared<View>()) {}

NativeWindowImpl::NativeWindowImpl(const std::shared_ptr<View>& view)
    : view_(view) {
  hWnd_ = ::CreateWindow(NativeWindowManager::WINDOW_CLASS_NAME,
                         DEFAULT_WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
                         CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr,
                         NativeApp::getInstance(), nullptr);
  SetWindowLong(hWnd_, GWL_USERDATA, reinterpret_cast<LONG>(this));

  context_ = DirectXRes::createContextFromHWnd(hWnd_);
}

void NativeWindowImpl::setView(const std::shared_ptr<View>& view) {
  view_ = view;
}

WindowState NativeWindowImpl::getWindowState() {
  WINDOWPLACEMENT window_placement;
  window_placement.length = sizeof(WINDOWPLACEMENT);
  ::GetWindowPlacement(hWnd_, &window_placement);
  WindowState state = WindowState::Shown;
  switch (window_placement.showCmd) {
    case SW_HIDE:
      state = WindowState::Hidden;
      break;
    case SW_SHOW:
      state = WindowState::Shown;
      break;
    case SW_MAXIMIZE:
      state = WindowState::Maximized;
      break;
    case SW_MINIMIZE:
      state = WindowState::Minimized;
      break;
  }
  return state;
}

void NativeWindowImpl::setWindowState(WindowState state) {
  switch (state) {
    case WindowState::Hidden:
      ::ShowWindow(hWnd_, SW_HIDE);
      break;
    case WindowState::Shown:
      ::ShowWindow(hWnd_, SW_SHOW);
      ::UpdateWindow(hWnd_);
      break;
    case WindowState::Restore:
      ::ShowWindow(hWnd_, SW_RESTORE);
      break;
    case WindowState::Maximized:
      ::ShowWindow(hWnd_, SW_MAXIMIZE);
      break;
    case WindowState::Minimized:
      ::ShowWindow(hWnd_, SW_MINIMIZE);
      break;
    default:
      break;
  }
}

void NativeWindowImpl::setTitle(const String& title) {
  ::SetWindowText(hWnd_, title.c_str());
}

String NativeWindowImpl::getTitle() const { return String{}; }

Rect NativeWindowImpl::getBounds() const {
  RECT rect;
  ::GetWindowRect(hWnd_, &rect);
  return {rect.left, rect.top, rect.right, rect.bottom};
}

void NativeWindowImpl::setBounds(const Rect& bounds) {
  ::MoveWindow(hWnd_, bounds.left(), bounds.top(), bounds.width(),
               bounds.height(), FALSE);
}
}  // namespace yuki
