#ifndef TYPEDEF_H_
#define TYPEDEF_H_

#ifndef TEXT

#ifdef UNICODE
#define TEXT(quote) L##quote

#else // !UNICODE
#define TEXT(quote) quote

#endif

#endif

#endif  // !TYPEDEF_H_