#include <core/app.h>
#include <ui/window.h>
#include <cstdio>
#include <memory>
#include "core/logger.h"


using namespace yuki;

class MyView : public View {
protected:
  void onRenderTargetChanged(Context2D* context) override {
    View::onRenderTargetChanged(context);
    brush_ = context->createSolidBrush(Color::Violet);
    font_ = context->createTextFormat(TEXT("Verdana"), 72.0f);
  }

  void onRender(Context2D* context) override {
    View::onRender(context);
    context->drawLine({2, 2, 100, 100}, brush_.get());
    context->drawCircle({100, 100, 25}, brush_.get());
    context->drawText(TEXT("Hello World!"), font_.get(), {0, 0, 1000, 1000},
                      brush_.get());
  }

  void sizeChangedEvent(SizeChangedEventArgs* args) override {
    const auto& size = args->getSize();
    Logger::trace()
      << TEXT("sizeChangedEvent: ")
      << size.width()
      << TEXT(", ")
      << size.height();
  }

  void sizeChangingEvent(SizeChangingEventArgs* args) override {
    const auto& rect = args->getRect();
    Logger::trace()
      << TEXT("SizeChangingEventArgs: ")
      << rect.left()
      << TEXT(", ")
      << rect.top()
      << TEXT(", ")
      << rect.right()
      << TEXT(", ")
      << rect.bottom();
  }

  void mouseButtonUpEvent(MouseEventArgs* args) override {
    const auto& position = args->position();
    Logger::trace()
      << TEXT("mouseButtonUpEvent: (")
      << position.x()
      << TEXT(", ")
      << position.y()
      << TEXT(") ")
      << (args->isControlDown() ? TEXT("Ctrl ") : TEXT(""))
      << (args->isShiftDown() ? TEXT("Shift ") : TEXT(""))
      << (args->isLButtonDown() ? TEXT("LButton ") : TEXT(""))
      << (args->isRButtonDown() ? TEXT("RButton ") : TEXT(""))
      << (args->isMButtonDown() ? TEXT("MButton") : TEXT(""));
  }

  void mouseButtonDownEvent(MouseEventArgs* args) override {
    const auto& position = args->position();
    Logger::trace()
      << TEXT("mouseButtonDownEvent: (")
      << position.x()
      << TEXT(", ")
      << position.y()
      << TEXT(") ")
      << (args->isControlDown() ? TEXT("Ctrl ") : TEXT(""))
      << (args->isShiftDown() ? TEXT("Shift ") : TEXT(""))
      << (args->isLButtonDown() ? TEXT("LButton ") : TEXT(""))
      << (args->isRButtonDown() ? TEXT("RButton ") : TEXT(""))
      << (args->isMButtonDown() ? TEXT("MButton") : TEXT(""));
  }

  void mouseButtonDoubleClickEvent(MouseEventArgs* args) override {
    const auto& position = args->position();
    Logger::trace()
      << TEXT("mouseButtonDoubleClickEvent: (")
      << position.x()
      << TEXT(", ")
      << position.y()
      << TEXT(") ")
      << (args->isControlDown() ? TEXT("Ctrl ") : TEXT(""))
      << (args->isShiftDown() ? TEXT("Shift ") : TEXT(""))
      << (args->isLButtonDown() ? TEXT("LButton ") : TEXT(""))
      << (args->isRButtonDown() ? TEXT("RButton ") : TEXT(""))
      << (args->isMButtonDown() ? TEXT("MButton") : TEXT(""));
  }

  void mouseMoveEvent(MouseEventArgs* args) override {
    const auto& position = args->position();
    Logger::trace()
      << TEXT("mouseMoveEvent: (")
      << position.x()
      << TEXT(", ")
      << position.y()
      << TEXT(") ")
      << (args->isControlDown() ? TEXT("Ctrl ") : TEXT(""))
      << (args->isShiftDown() ? TEXT("Shift ") : TEXT(""))
      << (args->isLButtonDown() ? TEXT("LButton ") : TEXT(""))
      << (args->isRButtonDown() ? TEXT("RButton ") : TEXT(""))
      << (args->isMButtonDown() ? TEXT("MButton") : TEXT(""));
  }

  void mouseWheelEvent(MouseEventArgs* args) override {
    const auto& position = args->position();
    Logger::trace()
      << TEXT("mouseWheelEvent: ")
      << TEXT("delta=")
      << args->delta()
      << TEXT(" (")
      << position.x()
      << TEXT(", ")
      << position.y()
      << TEXT(") ")
      << (args->isControlDown() ? TEXT("Ctrl ") : TEXT(""))
      << (args->isShiftDown() ? TEXT("Shift ") : TEXT(""))
      << (args->isLButtonDown() ? TEXT("LButton ") : TEXT(""))
      << (args->isRButtonDown() ? TEXT("RButton ") : TEXT(""))
      << (args->isMButtonDown() ? TEXT("MButton") : TEXT(""));
  }

  void keyDownEvent(KeyEventArgs* args) override {
    Logger::trace()
      << TEXT("keyDownEvent: ")
      << static_cast<int>(args->getKey());
  }

  void keyCharEvent(KeyCharEventArgs* args) override {
    Logger::trace()
      << TEXT("keyCharEvent: ")
      << args->getChar();
  }

  void keyUpEvent(KeyEventArgs* args) override {
    Logger::trace()
      << TEXT("keyUpEvent: ")
      << static_cast<int>(args->getKey());
  }

private:
  std::unique_ptr<Brush> brush_;
  std::unique_ptr<TextFormat> font_;
};

class TraceWindow : public Window {
public:
  explicit TraceWindow(std::shared_ptr<View> view) : Window(std::move(view)) {}

  void activateEvent(ActivateEventArgs* args) override {
    Logger::trace()
      << TEXT("activateEvent: ")
      << TEXT("isActivated=")
      << args->isActivated();
  }

  void closingEvent(ClosingEventArgs* args) override {
    Logger::trace()
      << TEXT("closingEvent");
  }

  void closedEvent() override {
    Logger::trace()
      << TEXT("ClosedEvent");
  }

  void windowStateChangeEvent(WindowStateChangedEventArgs* args) override {
    Logger::trace()
      << TEXT("windowStateChangeEvent: ")
      << TEXT("state=")
      << static_cast<int>(args->state());
  }

};

int main() {
  printf("Hello World!");
  App::init();

  auto view = std::make_shared<MyView>();
  auto window = std::make_unique<TraceWindow>(view);
  window->setTitle(TEXT("Hello World"));
  window->setBounds({0, 0, 400, 400});
  window->show();

  App::run();
  return 0;
}
