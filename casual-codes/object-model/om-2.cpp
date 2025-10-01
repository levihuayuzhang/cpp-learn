/**
 * @file om-2.cpp
 * @author Huayu Zhang (zhanghuayu.dev@gmail.com)
 * @brief object model - virtual base class
 * @version 0.1
 * @date 2025-09-21
 *
 * Copyright (c) 2025, Huayu Zhang <zhanghuayu.dev@gmail.com>.
 *
 */

// #include <cstdio>
#include <iostream>
#include "om-help.hpp"

struct Basic1 {
  uint64_t d11;
  virtual auto f() -> void {}
  virtual auto f2() -> void {}
  auto f3() -> void { std::cout << d11 << '\n'; }
};

void test() {
  std::cout << "ass" << '\n';
}

auto Demo() -> void {
  // auto (*fp)()->void = &test;
  // // std::cout << (void*)fp << '\n';
  // fp();
  // std::printf("%p\n", fp);

  auto (Basic1::*m_fp)()->void = &Basic1::f3;
  auto (*fp)(Basic1* b)->void = reinterpret_cast<auto (*&)(Basic1* b)->void>(m_fp);
  // void (*fp)(Basic1* b) = reinterpret_cast<void (*&)(Basic1* b)>(m_fp);
  Basic1 b;
  b.d11 = 5;
  fp(&b);
  ShowHex(m_fp);
}

auto main(int argc, const char* argv[]) -> int {
  Demo();
  return 0;
}