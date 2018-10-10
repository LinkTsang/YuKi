#pragma once
#include "core/object.h"

namespace yuki {
namespace graphic {
class Bitmap : public Object {
 public:
  Bitmap() = default;
  Bitmap(const Bitmap&) = default;
  Bitmap(Bitmap&&) = default;
  Bitmap& operator=(const Bitmap&) = default;
  Bitmap& operator=(Bitmap&&) = default;
  virtual ~Bitmap() = default;
};
}  // namespace graphic

enum class BitmapInterpolationMode { NearestNeighbor, Linear };
}  // namespace yuki
