#pragma once
#include <string_view>

class RpmlElement;

class RpmlAttribute;

class RpmlReader {
 public:
  RpmlReader();
  void load(const std::string_view& content);
};