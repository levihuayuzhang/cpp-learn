/**
 * @file concept-test.cpp
 * @author Huayu Zhang (zhanghuayu.dev@gmail.com)
 * @brief concept - compile time const which has bool as type.
 * @version 0.1
 * @date 2025-09-07
 *
 * Copyright (c) 2025, Huayu Zhang <zhanghuayu.dev@gmail.com>.
 *
 */

#include <type_traits>

template <typename T>
constexpr bool SmallType = sizeof(T) <= sizeof(void*);
template <typename T>
constexpr bool BigType = sizeof(T) > sizeof(void*);
// template <typename T>
// auto f(T t) -> std::enable_if<SmallType<T>, void> {}

template <typename T>
concept CouldCopy = requires {  // &&
  std::is_trivial_v<T>;
  // SmallType<T>;
  sizeof(T) <= sizeof(void*);
};  // || <s

template <typename T>
// requires true // maybe as default
// requires false
// requires SmallType<T>
// requires std::is_trivial_v<T>  // DEPRECATED_IN_CXX26
// requires std::is_trivially_copyable_v<T>
// requires std::is_trivially_default_constructible_v<T>
// requires std::is_trivial_v<T> && SmallType<T>
// requires requires {  // &&
//   std::is_trivial_v<T>;
//   SmallType<T>;
//   // sizeof(T) <= sizeof(void*);
// }  // || <some-bool-expr>
// requires CouldCopy<T>
  requires requires {
    CouldCopy<T>;
    std::is_pod_v<T>;
  }
auto f(T t) -> void {}

// template <typename T>
// requires BigType<T>
// auto f(T& t) -> void {}

struct Test {
  int arr[999];
};

auto Demo() -> void {
  int a = 5;
  f<int>(a);

  Test t;
  f<Test>(t);
}

auto main(int argc, const char* argv[]) -> int {
  Demo();

  return 0;
}