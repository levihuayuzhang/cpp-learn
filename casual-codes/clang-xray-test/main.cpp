#include <iostream>
[[clang::xray_always_instrument]]
int foo(int x) {
  return x * x;
}

int main() {
  for (int i = 0; i < 5; i++)
    std::cout << foo(i) << '\n';
}