#pragma once
#include <string>
#include <string_view>

namespace rpml {
class RpmlLoader {
 public:
  RpmlLoader();
  void load(const std::string_view& content);
};
}  // namespace rpml
