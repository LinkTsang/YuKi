#pragma once
#include <string>
#include "core/typedef.h"

namespace yuki {
#ifdef UNICODE
using String = std::wstring;
#else
using String = std::string;
#endif
}  // namespace yuki
