#include "ui/uielement.h"

namespace yuki {
void UIElement::onRender(Context2D* context) {
  context->clear(Color::WhiteSmoke);
}

void UIElement::onRenderTargetChanged(Context2D* context) {}
}
