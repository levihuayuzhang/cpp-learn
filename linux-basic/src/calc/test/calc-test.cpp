#include "calc.hpp"
#include <iostream>

auto main() -> int {
    std::cout << add_diy(3, 4) << '\n';
    std::cout << sub_diy(3, 4) << '\n';
    std::cout << mult_diy(3, 4) << '\n';
    std::cout << div_diy(3, 4) << '\n';
}