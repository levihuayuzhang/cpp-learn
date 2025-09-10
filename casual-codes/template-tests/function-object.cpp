// #include <functional>
#include <iostream>
#include <type_traits>
#include <utility>

// template <typename R, typename... Args>
// struct FuncTypeTraits {
//   using Head = R;
//   using Arg1T = FuncTypeTraits<Args...>;
// };

template <typename T, typename... Args>
concept IsInvocable = requires(T t, Args&&... args) { t(args...); };

template <typename FuncT>
class Function;

// partial specialization
template <typename R, typename... Args>
class Function<auto(Args...)->R> {
 public:
  // 1->function pointer
  Function(auto (*fp)(Args...)->R);

  // 2->imitation function (callable object)
  // template <IsInvocable FuncT>
  // template <typename T>
  // Function(IsInvocable<T> auto&& t);
  template <typename T>
  // requires IsInvocable<FuncT>
    requires std::is_invocable_v<T, Args...>
  Function(T&& t);

  // calling
  auto operator()(Args&&... args) -> R;

 private:
  auto (*fp_)(Args...) -> R;
};

template <typename R, typename... Args>
Function<R(Args...)>::Function(R (*fp)(Args...)) : fp_(fp) {}

template <typename R, typename... Args>
auto Function<R(Args...)>::operator()(Args&&... args) -> R {
  return fp_(std::forward<Args>(args)...);
}

auto sum(int a, int b) -> int {
  return a + b;
}

void Demo() {
  // Function<int, int, int> func;

  // function pointer
  // Function<auto(int, int)->int> f = sum;
  // auto (*fp)(int, int)->int = [](int a, int b) -> int { return a - b; };
  // Function<auto(int, int)->int> f = fp;
  Function<auto(int, int)->int> f =
      (auto (*)(int, int)->int)[](int a, int b)->int {
    return a - b;
  };
  std::cout << f(1, 3) << '\n';
}

auto main(int argc, const char* argv[]) -> int {
  Demo();

  return 0;
}