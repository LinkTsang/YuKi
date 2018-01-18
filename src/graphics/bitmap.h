#pragma once

namespace yuki {
class Bitmap {
 public:
  virtual ~Bitmap();
};
enum class BitmapInterpolationMode { NearestNeighbor, Linear };
}  // namespace yuki
