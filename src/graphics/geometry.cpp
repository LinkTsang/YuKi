#include "geometry.h"
#include "cmath"

namespace yuki {
namespace graphic {
Transform2D Transform2D::rotation(float theta) {
  return {std::cosf(theta),
          std::sinf(theta),
          -std::sinf(theta),
          -std::cosf(theta),
          0,
          0};
}

Transform2D Transform2D::rotation(float x, float y, float theta) {
  return {std::cosf(theta),
          std::sinf(theta),
          -std::sinf(theta),
          -std::cosf(theta),
          x * (1 - theta) + y * std::sinf(theta),
          -x * std::sinf(theta) + y * (1 - std::cosf(theta))};
}

Transform2D Transform2D::skew(float px, float py, float theta, float phi) {
  return {1, std::tanf(phi),         std::tanf(theta),
          1, -py * std::tanf(theta), -px * std::tanf(phi)};
}
}  // namespace graphic
}  // namespace yuki
