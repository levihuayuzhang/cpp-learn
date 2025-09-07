#include "calc.hpp"
#include <iostream>

auto main(int argc, const char *argv[]) -> int {
    std::cout << add_diy(3, 4) << '\n';
    std::cout << sub_diy(3, 4) << '\n';
    std::cout << mult_diy(3, 4) << '\n';
    std::cout << div_diy(3, 4) << '\n';
    for (int i = 0; i < argc; i++) {
        std::cout << "The " << i << " argument is: " << argv[i] << '\n';
    }
}