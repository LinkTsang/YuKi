#pragma once
#include "core/object.h"

namespace yuki {
class Bitmap : public YuKi::Object {
public:
  Bitmap() = default;
  Bitmap(const Bitmap&) = default;
  Bitmap(Bitmap&&) = default;
  Bitmap& operator=(const Bitmap&) = default;
  Bitmap& operator=(Bitmap&&) = default;
  virtual ~Bitmap() = default;
};

enum class BitmapInterpolationMode { NearestNeighbor, Linear };
} // namespace yuki
