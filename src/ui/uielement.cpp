#include "ui/uielement.h"
namespace yuki {
void UIElement::onRender(Context2D* context) {
  context->begin();
  context->clear(Color::WhiteSmoke);
  context->end();
}
}
