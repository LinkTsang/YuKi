#pragma once

#ifndef TEXT

#ifdef UNICODE
#define TEXT(quote) L##quote

#else  // !UNICODE
#define TEXT(quote) quote

#endif

#endif
