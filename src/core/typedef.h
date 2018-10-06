#pragma once

#ifdef UNICODE
#ifndef TEXT
#define TEXT(quote) L##quote
#endif
namespace yuki {
typedef wchar_t Char;
}  // namespace yuki
#else  // !UNICODE
#define TEXT(quote) quote
namespace yuki {
typedef char Char;
}  // namespace yuki
#endif