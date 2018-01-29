#pragma once

#ifdef TEXT
#undef  TEXT
#endif

#ifdef UNICODE
#define TEXT(quote) L##quote
namespace yuki {
typedef wchar_t Char;
} // namespace yuki
#else  // !UNICODE
#define TEXT(quote) quote
namespace yuki {
typedef char Char;
} // namespace yuki
#endif