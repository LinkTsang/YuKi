#include "ui/uielement.h"
#include <algorithm>

namespace yuki {
/******************************************************************************
* class UIElement
******************************************************************************/
void UIElement::onRender(Context2D* context) {
  context->clear(Color::WhiteSmoke);
}

void UIElement::onRenderTargetChanged(Context2D* context) {}

/******************************************************************************
* class UIContainer
******************************************************************************/
UIContainer& UIContainer::add(UIElement* element) {
  elements_.emplace_back(element);
  return *this;
}

UIContainer& UIContainer::remove(const UIElement* element) {
  elements_.erase(std::remove_if(elements_.begin(), elements_.end(),
                                 [element](const auto other) {
                                   return element == other.get();
                                 }), elements_.end());
  return *this;
}

UIContainer& UIContainer::add(std::initializer_list<UIElement*> elements) {
  for (auto&& element : elements) {
    elements_.emplace_back(element);
  }
  return *this;
}

UIContainer::const_iterator UIContainer::begin() const {
  return const_iterator(elements_.cbegin());
}

UIContainer::const_iterator UIContainer::end() const {
  return const_iterator(elements_.cend());
}

void UIContainer::onRenderTargetChanged(Context2D* context) const {
  for (const auto& element : elements_) {
    element->onRenderTargetChanged(context);
  }
}

void UIContainer::onRender(Context2D* context) const {
  for (const auto& element : elements_) {
    element->onRender(context);
  }
}

Shape::Shape() : fillBrush_(std::make_unique<SolidColorBrush>(Color::White)) {}

void Shape::setFill(Brush* brush) {
  fillBrush_.reset(brush);
}

const Brush* Shape::getFill() const {
  return fillBrush_.get();
}

void Shape::setStroke(Brush* brush) {
  strokeBrush_.reset(brush);
}

const Brush* Shape::getStroke() const {
  return strokeBrush_.get();
}

void Rectangle::onRenderTargetChanged(Context2D* context) {}

void Rectangle::onRender(Context2D* context) {
  context->fillRect(bounds_, fillBrush_.get());
}
}
