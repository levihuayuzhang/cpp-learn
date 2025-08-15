#include <iostream>

#if defined(_LIBCPP_VERSION)
#define LIB "libc++"
#elif defined(__GLIBCXX__)
#define LIB "libstdc++"
#else
#define LIB "unknown"
#endif

auto main() -> int { std::cout << "Using " << LIB << '\n'; }