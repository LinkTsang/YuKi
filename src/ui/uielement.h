#pragma once
#include <vector>
#include "core/object.h"
#include "graphics/painter.h"

namespace yuki {
namespace ui {
using namespace graphic;
class Style : public Object {
 public:
  const Brush& foreground() const { return *foreground_.get(); }
  const Brush& background() const { return *foreground_.get(); }
  const Font& font() const { return font_; }

  void setForeground(const Brush& brush) { foreground_.reset(brush.clone()); }
  void setBackground(const Brush& brush) { background_.reset(brush.clone()); }
  void setFont(const Font& font) { font_ = font; }

 private:
  std::unique_ptr<Brush> foreground_;
  std::unique_ptr<Brush> background_;
  Font font_;
};

/******************************************************************************
 * class UIElement
 ******************************************************************************/
class UIElement : public Object {
 public:
  UIElement() = default;
  UIElement(const UIElement&) = delete;
  UIElement(UIElement&&) = default;
  UIElement& operator=(const UIElement&) = delete;
  UIElement& operator=(UIElement&&) = default;
  virtual ~UIElement() = default;

  const RectF& getBounds() const { return bounds_; }
  RectF& getBounds() { return bounds_; }
  void setBounds(const RectF& bounds) { bounds_ = bounds; }

 protected:
  virtual void onRenderTargetChanged(Context2D* context);
  virtual void onRender(Context2D* context);
  friend class NativeWindowManager;
  friend class Window;
  friend class UIContainer;

 private:
  UIElement* parent_;
  RectF bounds_;
};

/******************************************************************************
 * class UIContainer
 ******************************************************************************/
class UIContainer : public Object {
 public:
  using Container = std::vector<std::shared_ptr<UIElement>>;
  using Iterator = Container::const_iterator;

  UIContainer() = default;

  UIContainer& add(UIElement* element);
  UIContainer& add(std::initializer_list<UIElement*> elements);
  UIContainer& remove(const UIElement* element);

  class const_iterator {
   public:
    using iterator_category = std::input_iterator_tag;
    using value_type = UIElement*;
    using difference_type = std::ptrdiff_t;
    using pointer = UIElement**;
    using reference = UIElement*&;
    explicit const_iterator(const Iterator& iterator) : iterator_(iterator) {}

    const_iterator& operator++() {
      ++iterator_;
      return *this;
    }

    const_iterator operator++(int) {
      auto retval = *this;
      ++(*this);
      return retval;
    }

    bool operator==(const const_iterator other) const {
      return iterator_ == other.iterator_;
    }

    bool operator!=(const const_iterator other) const {
      return !(*this == other);
    }

    value_type operator*() const { return iterator_->get(); }

   private:
    Iterator iterator_;
  };

  const_iterator begin() const;
  const_iterator end() const;

  void onRenderTargetChanged(Context2D* context) const;
  void onRender(Context2D* context) const;

 private:
  Container elements_;
};

class Shape : public UIElement {
 public:
  Shape();
  void setFill(Brush* brush);
  const Brush* getFill() const;
  void setStroke(Brush* brush);
  const Brush* getStroke() const;

 protected:
  std::unique_ptr<Brush> fillBrush_;
  std::unique_ptr<Brush> strokeBrush_;
};

class Rectangle : public Shape {
 public:
  Rectangle(float left, float top, float right, float bottom)
      : bounds_(left, top, right, bottom) {}

  void setHeight(const float value) { bounds_.setHeight(value); }
  void setWidth(const float value) { bounds_.setWidth(value); }
  float height() const { return bounds_.height(); }
  float width() const { return bounds_.width(); }

 protected:
  void onRenderTargetChanged(Context2D* context) override;
  void onRender(Context2D* context) override;

 private:
  RectF bounds_;
};
}  // namespace ui
}  // namespace yuki