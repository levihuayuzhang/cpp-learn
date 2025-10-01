#include <format>
#include <iostream>

template <typename T>
auto ShowHex(const T& t) -> void {
  // group by byte
  const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&t);

  // column head
  std::cout << "   ";
  for (int i = 0; i < 16; i++) {
    std::cout << std::format("{:2X} ", i);
  }

  for (int i = 0; i < sizeof(t); i++) {
    if (i % 16 == 0) {
      std::cout << '\n';
      std::cout << i / 16 << "  ";
    }
    std::cout << std::format("{:02X} ", ptr[i]);
  }
}