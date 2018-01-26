#include "window.h"
#include <Windows.h>
#include "graphics/painter.h"
#include "platforms/windows/direct2d.h"
#include "platforms/windows/nativeapp.h"

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
    case WM_MOUSEMOVE: {
    } break;

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

void NativeWindowImpl::show() {
  ::ShowWindow(hWnd_, SW_SHOW);
  ::UpdateWindow(hWnd_);
}

void NativeWindowImpl::hide() { ::ShowWindow(hWnd_, SW_HIDE); }

void NativeWindowImpl::setTitle(const String& title) {
  ::SetWindowText(hWnd_, title.c_str());
}

void NativeWindowImpl::setView(const std::shared_ptr<View>& view) {
  view_ = view;
}

String NativeWindowImpl::getTitle() const { return String{}; }

void NativeWindowImpl::moveToCenter() {}

void NativeWindowImpl::moveTo(const Point& p) {}

void NativeWindowImpl::setWindowState(WindowState state) {
  switch (state) {
    case WindowState::Hidden:
      hide();
      break;
    case WindowState::Shown:
      show();
      break;
    default:
      break;
  }
}

Size NativeWindowImpl::getSize() {
  RECT rect;
  ::GetClientRect(hWnd_, &rect);
  return {rect.right - rect.left, rect.top - rect.bottom};
}
}  // namespace yuki
