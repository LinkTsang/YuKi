#pragma once
#include <string>
#include "core/typedef.h"

namespace yuki {
#ifdef UNICODE
class String : public std::wstring {
public:
  String() {

  }
  String(const String& string) : std::wstring(string) {}
  String(const wchar_t* string) : std::wstring(string) {}
};
#else
class String : public std::string {
public:
  String() {

}
  String(const String& string) : std::string(string) {}
  String(const char* string) : std::string(string) {}
};
#endif
}

