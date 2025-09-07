/**
 * @file sfinae-test.cpp
 * @author Huayu Zhang (zhanghuayu.dev@gmail.com)
 * @brief Substitution Failure is Not a Error.
 * @version 0.1
 * @date 2025-09-04
 *
 * Copyright (c) 2025, Huayu Zhang <zhanghuayu.dev@gmail.com>.
 *
 */

#include <iostream>
#include <type_traits>

template <bool Cond, typename T>
struct enable_if {};

// partial specialization
template <typename T>
struct enable_if<true, T> {
    using type = T;
};

template <bool Cond, typename T>
using enable_if_t = typename enable_if<Cond, T>::type;

struct Test {
    int arr[999];
};

template <typename T>
struct IsLessThanPtr {
    constexpr static bool value = sizeof(T) <= sizeof(void*);
};

template <typename T>
constexpr bool IsLessThanPtr_v = IsLessThanPtr<T>::value;

template <typename T>
struct Not {
    constexpr static bool value = !T::value;
};

template <typename T>
constexpr static bool Not_v = Not<T>::value;

template <typename T>
void f(enable_if_t<IsLessThanPtr_v<T>, T> t) {
    std::cout << "primitive type were selected...\n";
}

template <typename T>
void f(enable_if_t<Not_v<IsLessThanPtr<T>>, T>& t) {
    std::cout << "big size type were selected...\n";
}

void Demo() {
    Test t;
    f<Test>(t);

    f<int>(1);
}

int main(int argc, const char* argv[]) {
    Demo();

    return 0;
}