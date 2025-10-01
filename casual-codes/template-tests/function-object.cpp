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
Function<R(Args...)>::Function(T&& t) : fp_(nullptr), fn_(new Invoker<T, R, Args...>(std::forward<T>(t))) {}

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
  auto operator()() -> void { std::cout << a << '\n'; }

  /*virtual*/ auto f(int a, double b) const noexcept -> void { std::cout << a << ", " << b << '\n'; }
};

template <typename T>
struct IsMemberFunction {
  constexpr static bool value = false;
};

template <typename R, typename Obj, typename... Args>
struct IsMemberFunction<auto (Obj::*)(Args...)->R> {
  constexpr static bool value = true;
};

template <typename R, typename Obj, typename... Args>
struct IsMemberFunction<auto (Obj::*)(Args...) const->R> {
  constexpr static bool value = true;
};

template <typename R, typename Obj, typename... Args>
struct IsMemberFunction<auto (Obj::*)(Args...) noexcept -> R> {
  constexpr static bool value = true;
};

template <typename R, typename Obj, typename... Args>
struct IsMemberFunction<auto (Obj::*)(Args...) const noexcept -> R> {
  constexpr static bool value = true;
};

template <typename T>
constexpr static bool IsMemberFunction_v = IsMemberFunction<T>::value;

template <size_t... Seq>
struct SeqRes {};

template <typename Seq>
struct RemoveHead {};

template <size_t Head, size_t... Seq>
struct RemoveHead<SeqRes<Head, Seq...>> {
  using type = SeqRes<Seq...>;
};

template <size_t N, size_t... Seq>
struct MakeReq {
  using type = typename MakeReq<N - 1, N - 1, Seq...>::type;
};

template <size_t... Seq>
struct MakeReq<0, Seq...> {
  using type = SeqRes<Seq...>;
};

template <typename Func, typename... BindArgs>
class BindT {
 public:
  BindT(Func func, BindArgs&&... bind_args);

  template <typename... InvokeArgs>
  auto operator()(InvokeArgs&&... args) -> decltype(auto); /*->
       std::conditional_t< std::is_pointer_v<Obj>,
       decltype((std::declval<Obj>()->*std::declval<Func>())(
           std::declval<Args>()...)),
       decltype((std::declval<Obj>().*
                 std::declval<Func>())(std::declval<Args>()...))>;*/

 private:
  Func func_;
  // Obj&& obj_;
  std::tuple<BindArgs...> args_;

  template <size_t... Seq, typename... InvokeArgs>
  auto RealInvoke(SeqRes<Seq...> seq, InvokeArgs&&... args) -> decltype(auto);

  template <size_t... Seq, typename... InvokeArgs>
  auto MemberFuncRealInvoke(SeqRes<Seq...> seq, InvokeArgs&&... args) -> decltype(auto);
};

template <typename Func, typename... BindArgs>
template <size_t... Seq, typename... InvokeArgs>
auto BindT<Func, BindArgs...>::RealInvoke(SeqRes<Seq...> seq, InvokeArgs&&... args) -> decltype(auto) {
  return func_(std::get<Seq>(args_)..., std::forward<InvokeArgs>(args)...);
}

template <typename Func, typename... BindArgs>
template <size_t... Seq, typename... InvokeArgs>
auto BindT<Func, BindArgs...>::MemberFuncRealInvoke(SeqRes<Seq...> seq, InvokeArgs&&... args) -> decltype(auto) {
  auto&& obj = std::get<0>(args_);
  using RealObj = std::remove_reference_t<decltype(obj)>;
  if constexpr (std::is_pointer_v<RealObj>) {
    return (obj->*func_)(std::get<Seq>(args_)..., std::forward<InvokeArgs>(args)...);
  } else {
    return (obj.*func_)(std::get<Seq>(args_)..., std::forward<InvokeArgs>(args)...);
  }
}

template <typename Func, typename... BindArgs>
template <typename... InvokeArgs>
auto BindT<Func, BindArgs...>::operator()(InvokeArgs&&... args) -> decltype(auto) {
  using Seq = typename MakeReq<sizeof...(BindArgs)>::type;
  if constexpr (IsMemberFunction_v<Func>) {
    using RHSeq = typename RemoveHead<Seq>::type;
    return this->MemberFuncRealInvoke(RHSeq{}, std::forward<InvokeArgs>(args)...);
  } else {
    return this->RealInvoke(Seq{}, std::forward<InvokeArgs>(args)...);
  }
}

template <typename Func, typename... BindArgs>
BindT<Func, BindArgs...>::BindT(Func func, BindArgs&&... bind_args)
    : func_(func), args_(std::forward<BindArgs>(bind_args)...) {}

template <typename Func, typename... BindArgs>
BindT(Func func, BindArgs&&... bind_args) -> BindT<Func, BindArgs...>;  // type derivation guide

void f1(int a, int b, int c) {
  std::cout << a << ", " << b << ", " << c << '\n';
}

void Demo() {
  // function pointer
  // Function<auto(int, int)->int> f = sum;
  // auto (*fp)(int, int)->int = [](int a, int b) -> int { return a - b; };
  // Function<auto(int, int)->int> f = fp;
  // Function<auto(int, int)->int> f = (auto (*)(int, int)->int)[](int a, int b)->int {
  //   return a - b;
  // };
  // std::cout << f(1, 3) << '\n';

  // no parameters
  // lambda expr
  // int a = 5;
  // auto ff = [&a]() -> void { a = 7; };
  // // ff();
  // Function<auto()->void> fn1(ff);
  // fn1();
  // std::cout << a << '\n';
  // // callable object
  // Pa p{3};
  // Function<auto()->void> fn2(p);
  // fn2();

  // // parameter of int
  // // not using universal ref && when calling parameter forwaring
  // auto fff = [](int arg) -> void { std::cout << arg << '\n'; };
  // Function<auto(int)->void> fnn = fff;
  // int b = 4;
  // fff(b);  // call lambda directly
  // // if using auto operator()(Args&&... args) -> R; at calling
  // fnn(5);  // pass in 5 is rval, Args is int, then args is int && -> int&&, ok
  // fnn(b);  // pass in b is lval, Args is int&, then args is int& && -> int&
  // // `calling` not work, because the && universal ref (forwarding ref)
  // // would turn function parameter type int->int& when passing
  // // the inner function call's parameter (int&) not compatible with
  // // outer function signature (template parameter) (int)
  // // use auto operator()(Args... args) -> R instead

  // auto (Pa::*mem_ptr)(int, double) const noexcept -> void = &Pa::f;
  // (p.*mem_ptr)(2, 3.5);
  // // BindT<auto (Pa::*)() const noexcept -> void, Pa&> bin(mem_ptr, p);
  // BindT bin(mem_ptr, p);
  // bin(1, 2.5);
  // Function<auto(int, double)->void> f4(bin);
  // f4(33, 4.4);
  // Function<auto(int, double)->void> f5 = BindT(&Pa::f, p);
  // f5(66, 8.8);

  // BindT bind_f1(f1, 3, 5);
  // bind_f1(8);
  // bind_f1(9);

  Pa p;
  auto bind_f = BindT(&Pa::f, p, 3);
  bind_f(7.6);
  Function<auto(double)->void> f2 = bind_f;
  f2(8.6);
}

auto main(int argc, const char* argv[]) -> int {
  Demo();
  return 0;
}