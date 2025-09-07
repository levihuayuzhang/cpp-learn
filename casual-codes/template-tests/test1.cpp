//
//  main.cpp
//  template-test
//
//  Created by Huayu Zhang on 9/3/25.
//

#include <cstring>
#include <iostream>
#include <string>

template <typename T>
auto sum(T& a, const T& b) -> T {
    return a + b;
}

template <>
auto sum<char*>(char*& a, char* const& b) -> char* {
    return std::strcat(a, b);
}

struct Test {
    int a, b;
};

template <>
auto sum<Test>(Test& a, const Test& b) -> Test {
    a.a += b.a;
    a.b += b.b;

    return a;
}

auto main(int argc, const char* argv[]) -> int {
    std::string ass = "aa";
    std::cout << sum<std::string>(ass, "bb") << '\n';

    int shit = 11;
    std::cout << sum<int>(shit, 22) << '\n';

    char a[] = "aa";
    char b[] = "cc";
    char* p1 = a;
    char* p2 = b;
    std::cout << sum(p1, p2) << '\n';

    Test test1{.a = 3, .b = 4};
    Test test2{.a = 5, .b = 6};
    Test test_rt = sum(test1, test2);
    std::cout << test_rt.a << '\n' << test_rt.b << '\n';

    return 0;
}
