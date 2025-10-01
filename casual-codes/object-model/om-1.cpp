#include <cstdint>
#include "om-help.hpp"

struct Basic1 {
  uint64_t d11;
  // uint32_t d12;

  // auto basic_f() -> void;
};
// auto Basic1::basic_f() -> void {
//   d12 = 9;
// }

struct Basic2 {
  uint64_t d21;
  // uint32_t d22;

  // auto basic_f() -> void;
};

// class -> data offset
// struct Test : Basic1, Basic2 {
struct Test : Basic2, Basic1 {
  uint8_t a;
  uint32_t b;
  uint32_t c;
  uint16_t d;

  // auto f() -> void;
};

// auto Test::f() -> void {
//   std::cout << d11 << '\n';
// }

void Demo() {
  // uint64_t a = 0x12345678;
  // ShowHex(a);
  // int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  // ShowHex(arr);
  // Test t{1, 2, 3, 4};
  // ShowHex(t);
  // uint8_t arr[16] = {0x01, 0, 0, 0, 0x02, 0};
  // // ShowHex(arr);
  // Test* t = reinterpret_cast<Test*>(arr);
  // std::cout << t->b;

  Test t;
  std::memset(&t, 0, sizeof(t));
  t.a = 0x99999999;  // trim
  t.b = 0x12345678;
  t.c = 0x66666666;
  t.d = 0x88888888;  // trim
  t.d11 = 0x1111111111111111;
  t.d21 = 0x5555555555555555;
  ShowHex(t);
  // t.basic_f();
  // t.f();
  // ShowHex(t);
}

auto main(int argc, const char* argv[]) -> int {
  Demo();
  return 0;
}