// #include <functional>
#include <iostream>
#include <type_traits>
#include <utility>

template <typename R, typename... Args>
struct BasicInvoker {
  virtual ~BasicInvoker() = default;
  virtual auto Invoke(Args&&... args) const -> R = 0;
};

template <typename T, typename R, typename... Args>
struct Invoker : BasicInvoker<R, Args...> {
  Invoker(T&& t);
  auto Invoke(Args&&... args) const -> R override;

 private:
  T&& t_;
};

template <typename T, typename R, typename... Args>
Invoker<T, R, Args...>::Invoker(T&& t) : t_(std::forward<T>(t)) {}

template <typename T, typename R, typename... Args>
auto Invoker<T, R, Args...>::Invoke(Args&&... args) const -> R {
  // return t_.operator()(std::forward<Args>(args)...);
  return t_(std::forward<Args>(args)...);
}

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
  // auto operator()(Args&&... args) -> R; // not using universal ref for
  // calling parameter forwaring
  auto operator()(Args... args) -> R;

 private:
  auto (*fp_)(Args...) -> R;
  BasicInvoker<R, Args...>* fn_;
};

template <typename R, typename... Args>
Function<R(Args...)>::Function(R (*fp)(Args...)) : fp_(fp), fn_(nullptr) {}

template <typename R, typename... Args>
template <typename T>
  requires std::is_invocable_v<T, Args...>
Function<R(Args...)>::Function(T&& t)
    : fp_(nullptr), fn_(new Invoker<T, R, Args...>(std::forward<T>(t))) {}

template <typename R, typename... Args>
// auto Function<R(Args...)>::operator()(Args&&... args) -> R { // not using
// universal ref for calling parameter forwaring
auto Function<R(Args...)>::operator()(Args... args) -> R {
  if (fp_ != nullptr) {
    return fp_(std::forward<Args>(args)...);
  }
  return fn_->Invoke(std::forward<Args>(args)...);
}

auto sum(int a, int b) -> int {
  return a + b;
}

struct Pa {
  int a;
  void operator()() { std::cout << a << '\n'; }
  void f() const noexcept {}
};

void Demo() {
  // function pointer
  // Function<auto(int, int)->int> f = sum;
  // auto (*fp)(int, int)->int = [](int a, int b) -> int { return a - b; };
  // Function<auto(int, int)->int> f = fp;
  Function<auto(int, int)->int> f =
      (auto (*)(int, int)->int)[](int a, int b)->int {
    return a - b;
  };
  std::cout << f(1, 3) << '\n';

  // no parameters
  // lambda expr
  int a = 5;
  auto ff = [&a]() -> void { a = 7; };
  // ff();
  Function<auto()->void> fn1(ff);
  fn1();
  std::cout << a << '\n';
  // callable object
  Pa p{3};
  Function<auto()->void> fn2(p);
  fn2();

  // parameter of int
  // not using universal ref && when calling parameter forwaring
  auto fff = [](int arg) -> void { std::cout << arg << '\n'; };
  Function<auto(int)->void> fnn = fff;
  int b = 4;
  fff(b);  // call lambda directly
  // if using auto operator()(Args&&... args) -> R; at calling
  fnn(5);  // pass in 5 is rval, Args is int, then args is int && -> int&&, ok
  fnn(b);  // pass in b is lval, Args is int&, then args is int& && -> int&
  // `calling` not work, because the && universal ref (forwarding ref)
  // would turn function parameter type int->int& when passing
  // the inner function call's parameter (int&) not compatible with
  // outer function signature (template parameter) (int)
  // use auto operator()(Args... args) -> R instead
}

auto main(int argc, const char* argv[]) -> int {
  Demo();
  return 0;
}