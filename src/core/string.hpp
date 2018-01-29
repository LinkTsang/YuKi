#pragma once
#include <string>
#include "core/typedef.h"

namespace yuki {
#ifdef UNICODE
using String = std::wstring;

template <typename T>
String ToString(T&& value) {
  return std::to_wstring(std::forward<T>(value));
}
#else
using String = std::string;

template<typename T>
String ToString(T&& value) {
  return std::to_string(std::forward<T>(value));
}
#endif

} // namespace yuki
