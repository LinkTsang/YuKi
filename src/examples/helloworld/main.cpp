#include <core/app.h>
#include <ui/window.h>
#include <cstdio>
#include <memory>
#include "core/logger.h"

using namespace yuki;
using namespace yuki::graphic;
using namespace yuki::ui;

enum class GridUnit { Absolute, Auto, Star };

class GridLength {
 public:
  explicit GridLength(GridUnit unit, float value = 0)
      : unit_(unit), value_(value) {}

  static const GridLength Auto;
  static const GridLength Star;

  GridUnit unit() const { return unit_; }
  void setUnit(GridUnit unit) { unit_ = unit; }
  void setValue(float value) { value_ = value; }
  float value() const { return value_; }

  bool isAbsolute() const { return unit_ == GridUnit::Absolute; }
  bool isStar() const { return unit_ == GridUnit::Star; }
  bool isAuto() const { return unit_ == GridUnit::Auto; }

 private:
  GridUnit unit_;
  float value_;
};

const GridLength GridLength::Auto{GridUnit::Auto};
const GridLength GridLength::Star{GridUnit::Star, 1};

class GridPane : public UIElement {
 public:
  GridPane() {}

  void addRowDefinition(GridLength height) {
    rowDefinitions_.push_back(height);
  }
  void addColumnDefinition(GridLength width) {
    columnDefinitions_.push_back(width);
  }

  size_t rowCount() const { return rowDefinitions_.size(); }
  size_t columnCount() const { return columnDefinitions_.size(); }

  void addChild(UIElement* element, uint32_t row = 0, uint32_t colomn = 0,
                uint32_t rowSpan = 1, uint32_t columnSpan = 1) {}

 protected:
  virtual void onRenderTargetChanged(Context2D* context) {}
  virtual void onRender(Context2D* context) {}

 private:
  std::vector<GridLength> rowDefinitions_;
  std::vector<GridLength> columnDefinitions_;
};

class MyView : public View {
 public:
  MyView() : brush_(new SolidColorBrush(Color::Violet)) {
    const auto rectangle = new Rectangle({100, 100, 200, 200});
    rectangle->setFill(new SolidColorBrush(Color::DarkViolet));
    children().add(rectangle);
  }

 protected:
  void onRenderTargetChanged(Context2D* context) override {
    View::onRenderTargetChanged(context);
    font_ = context->createTextFormat(TEXT("Verdana"), 72.0f);
  }

  void onRender(Context2D* context) override {
    View::onRender(context);
    // context->drawLine({2, 2, 100, 100}, brush_.get());
    // context->drawCircle({100, 100, 25}, brush_.get());
    // context->drawText(TEXT("Hello World!"), font_.get(), {0, 0, 1000, 1000},
    //                  brush_.get());

    context->pushClip({50, 50, 500, 500});
    context->drawLine({2, 2, 100, 100}, brush_.get());
    context->drawCircle({100, 100, 25}, brush_.get());
    context->drawText(TEXT("Hello World!"), font_.get(), {0, 0, 1000, 1000},
                      brush_.get());
    context->popClip();
  }

  void sizeChangedEvent(SizeChangedEventArgs* args) override {
    const auto& size = args->getSize();
    Logger::trace() << TEXT("sizeChangedEvent: ") << size.width() << TEXT(", ")
                    << size.height();
  }

  void sizeChangingEvent(SizeChangingEventArgs* args) override {
    const auto& rect = args->getRect();
    Logger::trace() << TEXT("SizeChangingEventArgs: ") << rect.left()
                    << TEXT(", ") << rect.top() << TEXT(", ") << rect.right()
                    << TEXT(", ") << rect.bottom();
  }

  void mouseButtonUpEvent(MouseEventArgs* args) override {
    const auto& position = args->position();
    Logger::trace() << TEXT("mouseButtonUpEvent: (") << position.x()
                    << TEXT(", ") << position.y() << TEXT(") ")
                    << (args->isControlDown() ? TEXT("Ctrl ") : TEXT(""))
                    << (args->isShiftDown() ? TEXT("Shift ") : TEXT(""))
                    << (args->isLButtonDown() ? TEXT("LButton ") : TEXT(""))
                    << (args->isRButtonDown() ? TEXT("RButton ") : TEXT(""))
                    << (args->isMButtonDown() ? TEXT("MButton") : TEXT(""));
  }

  void mouseButtonDownEvent(MouseEventArgs* args) override {
    const auto& position = args->position();
    Logger::trace() << TEXT("mouseButtonDownEvent: (") << position.x()
                    << TEXT(", ") << position.y() << TEXT(") ")
                    << (args->isControlDown() ? TEXT("Ctrl ") : TEXT(""))
                    << (args->isShiftDown() ? TEXT("Shift ") : TEXT(""))
                    << (args->isLButtonDown() ? TEXT("LButton ") : TEXT(""))
                    << (args->isRButtonDown() ? TEXT("RButton ") : TEXT(""))
                    << (args->isMButtonDown() ? TEXT("MButton") : TEXT(""));
  }

  void mouseButtonDoubleClickEvent(MouseEventArgs* args) override {
    const auto& position = args->position();
    Logger::trace() << TEXT("mouseButtonDoubleClickEvent: (") << position.x()
                    << TEXT(", ") << position.y() << TEXT(") ")
                    << (args->isControlDown() ? TEXT("Ctrl ") : TEXT(""))
                    << (args->isShiftDown() ? TEXT("Shift ") : TEXT(""))
                    << (args->isLButtonDown() ? TEXT("LButton ") : TEXT(""))
                    << (args->isRButtonDown() ? TEXT("RButton ") : TEXT(""))
                    << (args->isMButtonDown() ? TEXT("MButton") : TEXT(""));
  }

  void mouseMoveEvent(MouseEventArgs* args) override {
    const auto& position = args->position();
    Logger::trace() << TEXT("mouseMoveEvent: (") << position.x() << TEXT(", ")
                    << position.y() << TEXT(") ")
                    << (args->isControlDown() ? TEXT("Ctrl ") : TEXT(""))
                    << (args->isShiftDown() ? TEXT("Shift ") : TEXT(""))
                    << (args->isLButtonDown() ? TEXT("LButton ") : TEXT(""))
                    << (args->isRButtonDown() ? TEXT("RButton ") : TEXT(""))
                    << (args->isMButtonDown() ? TEXT("MButton") : TEXT(""));
  }

  void mouseWheelEvent(MouseEventArgs* args) override {
    const auto& position = args->position();
    Logger::trace() << TEXT("mouseWheelEvent: ") << TEXT("delta=")
                    << args->delta() << TEXT(" (") << position.x() << TEXT(", ")
                    << position.y() << TEXT(") ")
                    << (args->isControlDown() ? TEXT("Ctrl ") : TEXT(""))
                    << (args->isShiftDown() ? TEXT("Shift ") : TEXT(""))
                    << (args->isLButtonDown() ? TEXT("LButton ") : TEXT(""))
                    << (args->isRButtonDown() ? TEXT("RButton ") : TEXT(""))
                    << (args->isMButtonDown() ? TEXT("MButton") : TEXT(""));
  }

  void keyDownEvent(KeyEventArgs* args) override {
    Logger::trace() << TEXT("keyDownEvent: ")
                    << static_cast<int>(args->getKey());
  }

  void keyCharEvent(KeyCharEventArgs* args) override {
    Logger::trace() << TEXT("keyCharEvent: ") << args->getChar();
  }

  void keyUpEvent(KeyEventArgs* args) override {
    Logger::trace() << TEXT("keyUpEvent: ") << static_cast<int>(args->getKey());
  }

 private:
  std::unique_ptr<Brush> brush_;
  std::unique_ptr<TextFormat> font_;
};

class TraceWindow : public Window {
 public:
  explicit TraceWindow(std::shared_ptr<View> view) : Window(std::move(view)) {}

  void activateEvent(ActivateEventArgs* args) override {
    Logger::trace() << TEXT("activateEvent: ") << TEXT("isActivated=")
                    << args->isActivated();
  }

  void closingEvent(ClosingEventArgs* args) override {
    Logger::trace() << TEXT("closingEvent");
  }

  void closedEvent() override { Logger::trace() << TEXT("ClosedEvent"); }

  void windowStateChangeEvent(WindowStateChangedEventArgs* args) override {
    Logger::trace() << TEXT("windowStateChangeEvent: ") << TEXT("state=")
                    << static_cast<int>(args->state());
  }

  void movingEvent(WindowMovingEventArgs* args) override {
    const auto& rect = args->getRect();
    Logger::trace() << TEXT("movingEvent: ") << rect.left() << TEXT(", ")
                    << rect.top() << TEXT(", ") << rect.right() << TEXT(", ")
                    << rect.bottom();
  }

  void movedEvent(WindowMovedEventArgs* args) override {
    const auto& positon = args->getNewPosition();
    Logger::trace() << TEXT("movedEvent: ") << positon.x() << TEXT(", ")
                    << positon.y();
  }
};

class Label : public UIElement {
 public:
  Label() {}

  void setSize(SizeF size) { size_ = size; }
  SizeF size() const { return size_; }

  void setMaxSize(SizeF size) { maxSize_ = size; }
  SizeF maxSize() const { return maxSize_; }

  void setMinSize(SizeF size) { minSize_ = size; }
  SizeF minSize() const { return size_; }

  SizeF sizeHint() const { return {}; }

  void setText(const String& text) { text_ = text; }
  const String& text() const { return text_; }

 private:
  void onRender(Context2D* context) override {
    SolidColorBrush brush{Color::DarkOliveGreen};

    context->drawRect(RectF{size_}, &brush);
  }

 private:
  String text_;

  SizeF size_;
  SizeF minSize_;
  SizeF maxSize_;
};

int main() {
  printf("Hello World!");
  App::init();

  auto view = std::make_shared<MyView>();
  auto window = std::make_unique<TraceWindow>(view);
  window->setTitle(TEXT("Hello World"));
  window->setBounds({0, 0, 400, 400});
  window->show();

  auto pane = std::make_shared<GridPane>();
  pane->addRowDefinition(GridLength{GridUnit::Absolute, 80});
  pane->addRowDefinition(GridLength{GridUnit::Absolute, 40});
  pane->addRowDefinition(GridLength{GridUnit::Absolute, 30});
  pane->addColumnDefinition(GridLength{GridUnit::Absolute, 10});
  pane->addColumnDefinition(GridLength{GridUnit::Absolute, 20});

  App::run();
  return 0;
}
