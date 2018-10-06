#pragma once
#include "core/event.h"
#include "core/object.h"
#include "core/typedef.h"
#include "graphics/geometry.h"

namespace yuki {
/*******************************************************************************
* GUI Event
******************************************************************************/
class GuiEventArgs : public EventArgs {};

class ActivateEventArgs : public GuiEventArgs {
public:
  explicit ActivateEventArgs(const bool activated) : activated_(activated) {}
  bool isActivated() const { return activated_; }
private:
  bool activated_;
};

using ActivateEventHandler = EventHandler<Object*, ActivateEventArgs*>;
using ActivateEvent = Event<ActivateEventHandler>;

class ClosingEventArgs : public GuiEventArgs {
public:
  ClosingEventArgs() : closed_(true) {}
  void cancel() { closed_ = false; }
  void close() { closed_ = true; }
  bool isClosed() const { return closed_; }
private:
  bool closed_;
};

using ClosingEventHandler = EventHandler<Object*, ClosingEventArgs*>;
using ClosingEvent = Event<ClosingEventHandler>;
using ClosedEventHandler = EventHandler<Object*, EventArgs*>;
using ClosedEvent = Event<ClosedEventHandler>;

class SizeChangedEventArgs : public GuiEventArgs {
public:
  explicit SizeChangedEventArgs(const SizeF size) : size_(size) {}
  SizeF getSize() const { return size_; }
private:
  SizeF size_;
};

using SizeChangedEventHandler = EventHandler<Object*, SizeChangedEventArgs*>;
using SizeChangedEvent = Event<SizeChangedEventHandler>;

class SizeChangingEventArgs : public GuiEventArgs {
public:
  enum Edge {
    Left = 1,
    Right = 2,
    Top = 3,
    TopLeft = 4,
    TopRight = 5,
    Bottom = 6,
    BottomLeft = 7,
    BottomRight = 8,
  };

  explicit SizeChangingEventArgs(const Edge edge, const Rect rect) :
    edge_(edge),
    rect_(rect) {}

  const Rect& getRect() const { return rect_; }
  void setRect(const Rect& rect) { rect_ = rect; }

  const Edge& getEdge() const { return edge_; }
  void setRect(const Edge& edge) { edge_ = edge; }
private:
  Edge edge_;
  Rect rect_;
};

using SizeChangingEventHandler = EventHandler<Object*, SizeChangingEventArgs*>;
using SizeChangingEvent = Event<SizeChangingEventHandler>;

enum class WindowState { Shown, Hidden, Restore, Maximized, Minimized };

class WindowStateChangedEventArgs : public EventArgs {
public:
  explicit
  WindowStateChangedEventArgs(const WindowState state) : state_(state) { }

  WindowState state() const { return state_; }
private:
  WindowState state_;
};

class WindowMovingEventArgs : public GuiEventArgs {
public:
  explicit WindowMovingEventArgs(const Rect& rect) : rect_(rect) {}
  const Rect& getRect() const { return rect_; }
  void setRect(const Rect& rect) { rect_ = rect; }
private:
  Rect rect_;
};

class WindowMovedEventArgs : public GuiEventArgs {
public:
  explicit WindowMovedEventArgs(const Point& position) :
    position_(position) {}

  //The x and y coordinates of the upper - left corner of the client area of the window.
  const Point& getNewPosition() const {
    return position_;
  }

private:
  Point position_;
};

/*******************************************************************************
* Mouse Event
******************************************************************************/
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
  MouseEventArgs(const Point position, const int mouseKeyState, const int delta)
    : position_(position),
      mouseKeyState_(mouseKeyState),
      delta_(delta) {};
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

using MouseEventHandler = EventHandler<Object*, MouseEventArgs*>;
using MouseEvent = Event<MouseEventHandler>;

/*******************************************************************************
* Key Event
******************************************************************************/
enum class Key {
  // Left mouse button  
  LButton = 0x01,
  // Right mouse button
  RButton = 0x02,
  // Control-break processing
  Cancel = 0x03,
  // Middle mouse button (three-button mouse)
  Mbutton = 0x04,
  // X1 mouse button
  Xbutton1 = 0x05,
  // X2 mouse button
  Xbutton2 = 0x06,
  // BACKSPACE key
  Backspace = 0x08,
  // TAB key
  Tab = 0x09,
  // CLEAR key
  Clear = 0x0C,
  // ENTER key
  Return = 0x0D,
  // SHIFT key
  Shift = 0x10,
  // CTRL key
  Control = 0x11,
  // ALT key
  Alt = 0x12,
  // PAUSE key
  Pause = 0x13,
  // CAPS LOCK key
  Capital = 0x14,
  // IME Kana mode
  Kana = 0x15,
  // Hanguel mode
  Hanguel = 0x15,
  // IME Junja mode
  Junja = 0x17,
  // IME final mode
  Final = 0x18,
  // IME Hanja mode
  Hanja = 0x19,
  // IME Kanji mode
  Kanji = 0x19,
  // ESC key
  Escape = 0x1B,
  // IME convert
  Convert = 0x1C,
  // IME nonconvert
  NonConvert = 0x1D,
  // IME accept
  Accept = 0x1E,
  // IME mode change request
  Modechange = 0x1F,
  // SPACEBAR
  Space = 0x20,
  // PAGE UP key
  Prior = 0x21,
  // PAGE DOWN key
  Next = 0x22,
  // END key
  End = 0x23,
  // HOME key
  Home = 0x24,
  // LEFT ARROW key
  Left = 0x25,
  // UP ARROW key
  Up = 0x26,
  // RIGHT ARROW key
  Right = 0x27,
  // DOWN ARROW key
  Down = 0x28,
  // SELECT key
  Select = 0x29,
  // PRINT key
  Print = 0x2A,
  // EXECUTE key
  Execute = 0x2B,
  // PRINT SCREEN key
  Snapshot = 0x2C,
  // INS key
  Insert = 0x2D,
  // DEL key
  Delete = 0x2E,
  // HELP key
  Help = 0x2F,
  // Num0 Key
  Num0 = 0x30,
  // Num1 Key
  Num1 = 0x31,
  // Num2 Key
  Num2 = 0x32,
  // Num3 Key
  Num3 = 0x33,
  // Num4 Key
  Num4 = 0x34,
  // Num5 Key
  Num5 = 0x35,
  // Num6 Key
  Num6 = 0x36,
  // Num7 Key
  Num7 = 0x37,
  // Num8 Key
  Num8 = 0x38,
  // Num9 Key
  Num9 = 0x39,
  // A Key
  A = 0x41,
  // B Key
  B = 0x42,
  // C Key
  C = 0x43,
  // D Key
  D = 0x44,
  // E Key
  E = 0x45,
  // F Key
  F = 0x46,
  // G Key
  G = 0x47,
  // H Key
  H = 0x48,
  // I Key
  I = 0x49,
  // J Key
  J = 0x4A,
  // K Key
  K = 0x4B,
  // L Key
  L = 0x4C,
  // M Key
  M = 0x4D,
  // N Key
  N = 0x4E,
  // O Key
  O = 0x4F,
  // P Key
  P = 0x50,
  // Q Key
  Q = 0x51,
  // R Key
  R = 0x52,
  // S Key
  S = 0x53,
  // T Key
  T = 0x54,
  // U Key
  U = 0x55,
  // V Key
  V = 0x56,
  // W Key
  W = 0x57,
  // X Key
  X = 0x58,
  // Y Key
  Y = 0x59,
  // Z Key
  Z = 0x5A,
  // Left Windows key (Natural keyboard)
  LWin = 0x5B,
  // Right Windows key (Natural keyboard)
  RWin = 0x5C,
  // Applications key (Natural keyboard)
  Apps = 0x5D,
  // Computer Sleep key
  Sleep = 0x5F,
  // Numeric keypad 0 key
  NumPad0 = 0x60,
  // Numeric keypad 1 key
  NumPad1 = 0x61,
  // Numeric keypad 2 key
  NumPad2 = 0x62,
  // Numeric keypad 3 key
  NumPad3 = 0x63,
  // Numeric keypad 4 key
  NumPad4 = 0x64,
  // Numeric keypad 5 key
  NumPad5 = 0x65,
  // Numeric keypad 6 key
  NumPad6 = 0x66,
  // Numeric keypad 7 key
  NumPad7 = 0x67,
  // Numeric keypad 8 key
  NumPad8 = 0x68,
  // Numeric keypad 9 key
  NumPad9 = 0x69,
  // Multiply key
  Multiply = 0x6A,
  // Add key
  Add = 0x6B,
  // Separator key
  Separator = 0x6C,
  // Subtract key
  Subtract = 0x6D,
  // Decimal key
  Decimal = 0x6E,
  // Divide key
  Divide = 0x6F,
  // F1 key
  F1 = 0x70,
  // F2 key
  F2 = 0x71,
  // F3 key
  F3 = 0x72,
  // F4 key
  F4 = 0x73,
  // F5 key
  F5 = 0x74,
  // F6 key
  F6 = 0x75,
  // F7 key
  F7 = 0x76,
  // F8 key
  F8 = 0x77,
  // F9 key
  F9 = 0x78,
  // F10 key
  F10 = 0x79,
  // F11 key
  F11 = 0x7A,
  // F12 key
  F12 = 0x7B,
  // F13 key
  F13 = 0x7C,
  // F14 key
  F14 = 0x7D,
  // F15 key
  F15 = 0x7E,
  // F16 key
  F16 = 0x7F,
  // F17 key
  F17 = 0x80,
  // F18 key
  F18 = 0x81,
  // F19 key
  F19 = 0x82,
  // F20 key
  F20 = 0x83,
  // F21 key
  F21 = 0x84,
  // F22 key
  F22 = 0x85,
  // F23 key
  F23 = 0x86,
  // F24 key
  F24 = 0x87,
  // NUM LOCK key
  NumLock = 0x90,
  // SCROLL LOCK key
  Scroll = 0x91,
  // Left SHIFT key
  LShift = 0xA0,
  // Right SHIFT key
  RShift = 0xA1,
  // Left CONTROL key
  LControl = 0xA2,
  // Right CONTROL key
  RControl = 0xA3,
  // Left MENU key
  LMenu = 0xA4,
  // Right MENU key
  RMenu = 0xA5,
  // Browser Back key
  BrowserBack = 0xA6,
  // Browser Forward key
  BrowserForward = 0xA7,
  // Browser Refresh key
  BrowserRefresh = 0xA8,
  // Browser Stop key
  BrowserStop = 0xA9,
  // Browser Search key
  BrowserSearch = 0xAA,
  // Browser Favorites key
  BrowserFavorites = 0xAB,
  // Browser Start and Home key
  BrowserHome = 0xAC,
  // Volume Mute key
  VolumeMute = 0xAD,
  // Volume Down key
  VolumeDown = 0xAE,
  // Volume Up key
  VolumeUp = 0xAF,
  // Next Track key
  MediaNextTrack = 0xB0,
  // Previous Track key
  MediaPrevTrack = 0xB1,
  // Stop Media key
  MediaStop = 0xB2,
  // Play/Pause Media key
  MediaPlayPause = 0xB3,
  // Start Mail key
  LaunchMail = 0xB4,
  // Select Media key
  LaunchMediaSelect = 0xB5,
  // Start Application 1 key
  LaunchApp1 = 0xB6,
  // Start Application 2 key
  LaunchApp2 = 0xB7,
  // Used for miscellaneous characters, it can vary by keyboard. For the US standard keyboard, the ';:' key
  Oem1 = 0xBA,
  // For any country/region, the '+' key
  OemPlus = 0xBB,
  // For any country/region, the ',' key
  OemComma = 0xBC,
  // For any country/region, the '-' key
  OemMinus = 0xBD,
  // For any country/region, the '.' key
  OemPeriod = 0xBE,
  // Used for miscellaneous characters, it can vary by keyboard. For the US standard keyboard, the '/?' key
  Oem2 = 0xBF,
  // Used for miscellaneous characters, it can vary by keyboard. For the US standard keyboard, the '`~' key
  Oem3 = 0xC0,
  // Used for miscellaneous characters, it can vary by keyboard. For the US standard keyboard, the '[{' key
  Oem4 = 0xDB,
  // Used for miscellaneous characters, it can vary by keyboard. For the US standard keyboard, the '\|' key
  Oem5 = 0xDC,
  // Used for miscellaneous characters, it can vary by keyboard. For the US standard keyboard, the ']}' key
  Oem6 = 0xDD,
  // Used for miscellaneous characters, it can vary by keyboard. For the US standard keyboard, the 'single-quote/double-quote' key
  Oem7 = 0xDE,
  // Used for miscellaneous characters, it can vary by keyboard.
  Oem8 = 0xDF,
  // Either the angle bracket key or the backslash key on the RT 102-key keyboard
  Oem102 = 0xE2,
  // IME PROCESS key
  Processkey = 0xE5,
  // Used to pass Unicode characters as if they were keystrokes. 
  Packet = 0xE7,
  // Attn key
  Attn = 0xF6,
  // CrSel key
  CrSel = 0xF7,
  // ExSel key
  ExSel = 0xF8,
  // Erase EOF keyLSHIFT
  EraseEof = 0xF9,
  // Play key
  Play = 0xFA,
  // Zoom key
  Zoom = 0xFB,
  // PA1 key
  Pa1 = 0xFD,
  // Clear key
  OemClear = 0xFE,
};

class KeyEventArgs : public EventArgs {
public:
  explicit KeyEventArgs(Key key);
  Key getKey() const { return key_; }
  virtual bool isAltPressed() const = 0;
  virtual bool isCtrlPressed() const = 0;
  virtual bool isShiftPressed() const = 0;
  virtual bool isKeyPressed(Key key) const = 0;
private:
  Key key_;
};

using KeyEventHandler = EventHandler<Object*, KeyEventArgs*>;
using KeyEvent = Event<KeyEventHandler>;

class KeyCharEventArgs : public EventArgs {
 public:
  explicit KeyCharEventArgs(const Char c) : char_(c) {}

  Char getChar() const { return char_; }

 private:
  Char char_;
};
}
