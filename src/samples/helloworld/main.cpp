#include <core/app.h>
#include <ui/window.h>
#include <cstdio>
#include <memory>
using namespace yuki;

class MyView : public View {
 protected:
  void onRender(Context2D* context) override {
    View::onRender(context);

    if (brush == nullptr) {
      brush = context->createSolidBrush(Color::DarkViolet);
    }
    LineF line(2, 2, 100, 100);
    context->begin();
    context->drawLine(line, brush.get());
    context->drawCircle(CircleF{100, 100, 25}, brush.get());
    context->end();
  }

 public:
  std::unique_ptr<Brush> brush;
  virtual ~MyView() = default;
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