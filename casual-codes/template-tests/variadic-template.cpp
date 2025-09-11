/**
 * @file variadic-template.cpp
 * @author Huayu Zhang (zhanghuayu.dev@gmail.com)
 * @brief Simplifying recurse impl.
 * @version 0.1
 * @date 2025-09-06
 *
 * Copyright (c) 2025, Huayu Zhang <zhanghuayu.dev@gmail.com>.
 *
 */

// #include <cmath>
#include <iostream>

// template <typename T>
// T Sum(const T& a, const T& b) {
//     return a + b;
// }

// template <typename T>
// T SumAbs(const T& a) {
//     return std::abs(a);
// }

// three types of expansion:
// 1. directly (comma)
// 2. nesting (nesting->comma)
// 3. custom operator (collapsing expression)
// template <typename T, typename... Args>
// T SumAbs(const T& t, const Args&... args) {
//     return std::abs(t) + SumAbs<Args...>(std::abs(args)...);
// }

// Monism expansion
template <typename... Args>
auto Sum(Args... args) {
  // calc left side first (... decide which side calc first)
  // ((1 + 2) + 3) + 4
  //    return (... + args);
  // return (args + ...);
  return (... - args);
  // return (args - ...);
}

// dualism expansion
template <typename... Args>
void Print(const Args&... args) {
  // cout & args
  (std::cout << ... << args) << '\n';
}

void Demo() {
  // std::cout << SumAbs(1, 2, 3, 4, 5) << '\n';
  std::cout << Sum(1, 2, 3, 4, 5) << '\n';
  Print(1, 2, 3, 4, 5);
}

int main(int argc, const char* argv[]) {
  Demo();

  return 0;
}