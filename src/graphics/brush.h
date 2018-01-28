#pragma once
#include "core/object.h"

namespace yuki {
class Brush : public YuKi::Object {
public:
  Brush() = default;
  Brush(const Brush&) = default;
  Brush(Brush&&) = default;
  Brush& operator=(const Brush&) = default;
  Brush& operator=(Brush&&) = default;
  virtual ~Brush() = 0;
};

class SolidColorBrush : public Brush { };

class LinearGradientBrush : public Brush { };

class RadialGradientBrush : public Brush { };

class BitmapBrush : public Brush { };
} // namespace yuki
