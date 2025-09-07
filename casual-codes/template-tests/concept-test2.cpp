#include <concepts>
#include <cstdint>
#include <type_traits>

struct Test1 {
  int f;
};

struct Test2 {
  // void f() {}  // should impl method f
  int f() { return 0; }  // should impl method f with return type of int
};

struct Test3 {};

// pre-c++20
// template <typename T>
// constexpr bool hasMemF =
//     std::is_void_v<std::void_t<decltype(std::declval<T>().f())>>;
// template <typename T>
// constexpr bool hasMemIntF =
//     std::is_same_v<decltype(std::declval<T>().f()), int>;

// template <typename T>
// constexpr bool hasMemF = requires {
//   std::declval<T>().f();  // if legal, not really construct an object
// };
// template <typename T>
// concept hasMemF = requires(T t) {
//   t.f();  // not really construct an object
// };
// template <typename T>
// constexpr bool HasMemIntF = requires(T t) {
//   std::is_same_v<decltype(t.f()), int>;  // not really construct an object
// };
template <typename T>
concept HasMemIntF = requires(T t) {
  {
    t.f()
  } -> std::same_as<int>;  // c++20 concept: type of return value of expr inside
                           // the {} pass to same_as, need #include <concepts>
};

// template <typename T>
// auto func(const T& t) -> std::enable_if<hasMemF<T>, void> {}  // pre-c++20
// template <typename T>
//   requires hasMemF<T>  // c++20
// auto func(const T& t) -> void {}
template <typename T>
  requires HasMemIntF<T>
auto func(const T& t) -> void {}

void Demo() {
  // Test1 t1;
  // func<Test1>(t1);

  Test2 t2;
  func<Test2>(t2);
}

auto main(int argc, const char* argv[]) -> int {
  Demo();

  return 0;
}