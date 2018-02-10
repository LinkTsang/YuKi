#include "window.h"
#include <Windows.h>
#include <Windowsx.h>
#include "platforms/windows/direct2d.h"
#include "platforms/windows/nativeapp.h"
#include "platforms/windows/userinput.h"

namespace yuki {
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
  wcex.hIcon = nullptr; // LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DEMO));
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = HBRUSH(COLOR_WINDOW + 1);
  wcex.lpszMenuName = nullptr; // MAKEINTRESOURCEW(IDC_DEMO);
  wcex.lpszClassName = WINDOW_CLASS_NAME;
  wcex.hIconSm =
    nullptr; // LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

  RegisterClassEx(&wcex);
}

LRESULT NativeWindowManager::WndProc(HWND hWnd, UINT message, WPARAM wParam,
                                     LPARAM lParam) {
  auto* nativeWindow = reinterpret_cast<NativeWindowImpl*>(
    GetWindowLong(hWnd, GWL_USERDATA));
  if (nativeWindow == nullptr) {
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  auto* w = nativeWindow->window_;
  auto v = w->getView();
  if (WM_MOUSEFIRST <= message && message <= WM_MOUSELAST) {
    MouseEventArgs event_args{
      {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)},
      GET_KEYSTATE_WPARAM(wParam),
      GET_WHEEL_DELTA_WPARAM(wParam)
    };
    switch (message) {
      case WM_MOUSEMOVE:
        v->mouseMoveEvent(&event_args);
        break;
      case WM_LBUTTONDOWN:
      case WM_RBUTTONDOWN:
      case WM_MBUTTONDOWN:
        v->mouseButtonDownEvent(&event_args);
        break;
      case WM_LBUTTONUP:
      case WM_RBUTTONUP:
      case WM_MBUTTONUP:
        v->mouseButtonUpEvent(&event_args);
        break;
      case WM_LBUTTONDBLCLK:
      case WM_RBUTTONDBLCLK:
      case WM_MBUTTONDBLCLK:
        v->mouseButtonDoubleClickEvent(&event_args);
        break;
      case WM_MOUSEWHEEL:
        v->mouseWheelEvent(&event_args);
        break;
      default:
        break;
    }
  }
  switch (message) {
    case WM_ACTIVATE: {
      if (LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE) {
        ActivateEventArgs args{true};
        w->activateEvent(&args);
      } else if (LOWORD(wParam) == WA_INACTIVE) {
        ActivateEventArgs args{false};
        w->activateEvent(&args);
      }
      break;
    }
    case WM_CLOSE: {
      ClosingEventArgs args;
      w->closingEvent(&args);
      if (args.isClosed()) {
        DestroyWindow(hWnd);
      }
      break;
    }
    case WM_DESTROY: {
      PostQuitMessage(0);
      break;
    }
    case WM_PAINT: {
      PAINTSTRUCT ps;
      BeginPaint(hWnd, &ps);
      auto context = nativeWindow->context_.get();
      context->beginDraw();
      v->onRender(context);
      context->endDraw();
      EndPaint(hWnd, &ps);
      break;
    }
    case WM_MOVING: {
      const auto pRect = reinterpret_cast<LPRECT>(lParam);
      WindowMovingEventArgs args{
        {pRect->left, pRect->top, pRect->right, pRect->bottom}
      };
      w->movingEvent(&args);
      const auto& newRect = args.getRect();
      pRect->left = newRect.left();
      pRect->top = newRect.top();
      pRect->right = newRect.right();
      pRect->bottom = newRect.bottom();
      break;
    }
    case WM_MOVE: {
      WindowMovedEventArgs args{{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)}};
      w->movedEvent(&args);
      break;
    }
    case WM_SIZE: {
      Size size{LOWORD(lParam), HIWORD(lParam)};
      const SizeF sizeF{float(size.width()), float(size.height())};
      auto context = nativeWindow->context_.get();
      context->resetSize(sizeF);
      v->onRenderTargetChanged(context);
      SizeChangedEventArgs args{size};
      v->sizeChangedEvent(&args);
      break;
    }
    case WM_SIZING: {
      const auto pRect = reinterpret_cast<LPRECT>(lParam);
      SizeChangingEventArgs args{
        static_cast<SizeChangingEventArgs::Edge>(wParam),
        {pRect->left, pRect->top, pRect->right, pRect->bottom}
      };
      v->sizeChangingEvent(&args);
      auto newRect = args.getRect();
      pRect->left = newRect.left();
      pRect->top = newRect.top();
      pRect->right = newRect.right();
      pRect->bottom = newRect.bottom();
      break;
    }
    case WM_SHOWWINDOW: {
      const auto shown = static_cast<BOOL>(wParam);
      WindowState state;
      if (shown) {
        state = WindowState::Shown;
      } else {
        state = WindowState::Hidden;
      }
      WindowStateChangedEventArgs args(state);
      w->windowStateChangeEvent(&args);
      break;
    }
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN: {
      WinKeyEventArgs args{static_cast<Key>(wParam)};
      v->keyDownEvent(&args);
      break;
    }
    case WM_CHAR:
    case WM_SYSCHAR: {
      KeyCharEventArgs args{Char(wParam)};
      v->keyCharEvent(&args);
      break;
    }
    case WM_KEYUP:
    case WM_SYSKEYUP: {
      WinKeyEventArgs args{static_cast<Key>(wParam)};
      v->keyUpEvent(&args);
      break;
    }
    case WM_TIMER: {
      break;
    }
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

/*******************************************************************************
 * class NativeWindowImpl
 ******************************************************************************/

const TCHAR NativeWindowImpl::DEFAULT_WINDOW_TITLE[] = TEXT("Test");

NativeWindowImpl::NativeWindowImpl(Window* window, std::shared_ptr<View> view) :
  hWnd_(::CreateWindow(NativeWindowManager::WINDOW_CLASS_NAME,
          DEFAULT_WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
          CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr,
          NativeApp::getInstance(), nullptr)),
  window_(window),
  view_(std::move(view)),
  context_(DirectXRes::createContextFromHWnd(hWnd_)) {
  SetWindowLong(hWnd_, GWL_USERDATA, reinterpret_cast<LONG>(this));
}

WindowState NativeWindowImpl::getWindowState() {
  WINDOWPLACEMENT window_placement;
  window_placement.length = sizeof(WINDOWPLACEMENT);
  GetWindowPlacement(hWnd_, &window_placement);
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
      ShowWindow(hWnd_, SW_HIDE);
      break;
    case WindowState::Shown:
      ShowWindow(hWnd_, SW_SHOW);
      UpdateWindow(hWnd_);
      break;
    case WindowState::Restore:
      ShowWindow(hWnd_, SW_RESTORE);
      break;
    case WindowState::Maximized:
      ShowWindow(hWnd_, SW_MAXIMIZE);
      break;
    case WindowState::Minimized:
      ShowWindow(hWnd_, SW_MINIMIZE);
      break;
    default:
      break;
  }
}

void NativeWindowImpl::setView(std::shared_ptr<View> view) {
  if (view) {
    view_ = std::move(view);
  } else {
    view_ = std::make_shared<View>();
  }
}

std::shared_ptr<View> NativeWindowImpl::getView() const {
  return view_;
}

void NativeWindowImpl::setTitle(const String& title) {
  ::SetWindowText(hWnd_, title.c_str());
}

String NativeWindowImpl::getTitle() const { return String{}; }

Rect NativeWindowImpl::getBounds() const {
  RECT rect;
  GetWindowRect(hWnd_, &rect);
  return {rect.left, rect.top, rect.right, rect.bottom};
}

void NativeWindowImpl::setBounds(const Rect& bounds) {
  RECT rect{bounds.left(), bounds.top(), bounds.right(), bounds.bottom()};
  const auto bMenu = ::GetMenu(hWnd_) != nullptr;
  const auto dwStyle = ::GetWindowLong(hWnd_, GWL_STYLE);
  const auto dwExStyle = ::GetWindowLong(hWnd_, GWL_EXSTYLE);
  ::AdjustWindowRectEx(&rect, dwStyle, bMenu, dwExStyle);
  ::MoveWindow(hWnd_, bounds.left(),
               bounds.top(),
               rect.right - rect.left,
               rect.bottom - rect.top,
               TRUE);
}

} // namespace yuki
