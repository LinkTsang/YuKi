#ifndef YUKI_BRUSH_H_
#define YUKI_BRUSH_H_

namespace yuki {
class Brush {
 public:
  virtual ~Brush() = 0;
};

class SolidColorBrush : public Brush {};

class LinearGradientBrush : public Brush {};

class RadialGradientBrush : public Brush {};

class BitmapBrush : public Brush {};
}
#endif  // !YUKI_BRUSH_H_