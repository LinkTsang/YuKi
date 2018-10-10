#pragma once
#include "core/object.h"
#include "graphics/color.h"

namespace yuki {
namespace graphic {
enum class BrushStyle { SolidColor, LinearGradient, RadialGradient, Bitmap };

class Brush : public Object {
 public:
  Brush(const Brush&) = default;
  Brush(Brush&&) = default;
  Brush& operator=(const Brush&) = default;
  Brush& operator=(Brush&&) = default;
  virtual ~Brush() = 0;
  virtual Brush* clone() const = 0;

  BrushStyle style() const { return style_; }

 protected:
  explicit Brush(const BrushStyle type) : style_(type) {}
  BrushStyle style_;
};

class SolidColorBrush : public Brush {
 public:
  explicit SolidColorBrush(const ColorF color)
      : Brush(BrushStyle::SolidColor), color_(color) {}
  ~SolidColorBrush(){};
  SolidColorBrush* clone() const override {
    return new SolidColorBrush(color_);
  }

  ColorF getColor() const { return color_; }
  void setColor(const ColorF& color) { color_ = color; }

 private:
  ColorF color_;
};

class LinearGradientBrush : public Brush {};

class RadialGradientBrush : public Brush {};

class BitmapBrush : public Brush {};
}  // namespace graphic
}  // namespace yuki
