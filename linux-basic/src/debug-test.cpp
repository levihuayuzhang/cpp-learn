#include <iostream>

#define NUMBER 3

auto main() -> int {
    std::cout << "shit" << '\n';
#ifdef DEBUG
    std::cout << "Currently in DEBUG...";
#endif
}
