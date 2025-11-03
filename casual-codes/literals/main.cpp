#include <cstdint>
#include <iostream>

constexpr auto operator""_killo(unsigned long long n) -> uint64_t {
  return n * 1024;
}

constexpr auto operator""_micro(long double n) -> double {
  return n * 1e-6;
}

auto Demo() -> void {
  const int a = 5;
  constexpr int c = 7;
  double d = 5.6;
  float f = 7.3f;
  unsigned long l = 655ul;

  uint64_t capacity = 5_killo;
  std::cout << capacity << '\n';

  double m = 6.8_micro;
  std::cout << m << '\n';
}

auto main(int argc, const char* argv[]) -> int {
  Demo();
}