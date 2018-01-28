#pragma once

namespace YuKi {
class Object {
public:
  Object() = default;
  Object(const Object&) = default;
  Object(Object&&) = default;
  Object& operator=(const Object&) = default;
  Object& operator=(Object&&) = default;
  virtual ~Object() = default;
};
} // namespace YuKi
