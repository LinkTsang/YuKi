#include <core/app.h>
#include <ui/window.h>
#include <cstdio>
#include <memory>
using namespace yuki;

class MyView : public View {
 public:
  virtual ~MyView() = default;

 protected:
  void onRender(Context2D* context) override {
    View::onRender(context);

    if (!initialized_) {
      brush_ = context->createSolidBrush(Color::DarkViolet);
      font_ = context->createTextFormat(TEXT("Verdana"), 72.0f);
    }

    context->begin();
    context->drawLine({2, 2, 100, 100}, brush_.get());
    context->drawCircle({100, 100, 25}, brush_.get());
    context->drawText(TEXT("Hello World!"), font_.get(), {0, 0, 1000, 1000},
                      brush_.get());
    context->end();
  }

 private:
  bool initialized_ = false;
  std::unique_ptr<Brush> brush_;
  std::unique_ptr<TextFormat> font_;
};

int main() {
  printf("Hello World!");
  App::init();

  auto view = std::make_shared<MyView>();
  auto window = std::make_unique<Window>(view);
  window->setTitle(TEXT("Hello World"));
  window->show();

  App::run();
  return 0;
}