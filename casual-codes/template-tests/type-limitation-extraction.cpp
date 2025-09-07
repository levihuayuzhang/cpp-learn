/**
 * @file type-limitation-extraction.cpp
 * @author Huayu Zhang (zhanghuayu.dev@gmail.com)
 * @brief impl type limitation `without` concept(c++20)
 * @version 0.1
 * @date 2025-09-06
 *
 * Copyright (c) 2025, Huayu Zhang <zhanghuayu.dev@gmail.com>.
 *
 */
#include <iostream>
#include <type_traits>

// template <typename T>
// class Set {
//    public:
//     Set();
//     ~Set();

//     // compare/sorting happens here
//     void insert(const T& item);      // <
//     auto find(const T& item) -> T*;  // < and =

//    private:
//     T* buf;
// };

// // need to impl comparable, etc.
// struct Test {};

struct Test1 {
    // Test1(int a);
    int method();
    int member;
};

struct Test2 {
    void method();
};

struct Test3 {};

template <typename T>
struct RemoveRef {
    using type = T;
};

template <typename T>
struct RemoveRef<T&> {
    using type = T;
};

template <typename T>
struct RemoveRef<T&&> {
    using type = T;
};

template <typename T>
struct RemoveRef<const T&> {
    using type = const T;
};

template <typename T>
using RemoveRef_t = typename RemoveRef<T>::type;

template <typename T1, typename T2>
struct IsSame {
    constexpr static bool value = false;
};
template <typename T>
struct IsSame<T, T> {
    constexpr static bool value = true;
};

template <typename T1, typename T2>
constexpr bool IsSame_v = IsSame<T1, T2>::value;

template <typename T>
struct ToInt {
    // using type = int;
    using type = void;
};

template <typename T>
using ToInt_t = typename ToInt<T>::type;

// construct a object of T and call method()
// T have to implement void method() function
// if method() can be called
// judge whether the return value is type of int
// if not, then substitution failure
template <typename T>
// std::enable_if_t<std::is_same_v<decltype(T{}.method()), void>, void> f(const
// T& t) {}
// std::enable_if_t<IsSame_v<decltype(T{}.method()), void>, void> f(const T& t)
// {}
// std::enable_if_t<IsSame_v<decltype(static_cast<T*>(nullptr)->method()),void>,void>
// f(const T& t) {}
// std::enable_if_t<IsSame_v<decltype(std::declval<T>().method()), int>, void>
// f(
//     const T& t) {}

// the method() has to be impl, but the type of return value doesn't matter
// std::enable_if_t<
//     IsSame_v<typename ToInt<decltype(std::declval<T>().method())>::type,
//     int>, void>
// f(const T& t) {}
// std::enable_if_t<IsSame_v<ToInt_t<decltype(std::declval<T>().method())>,
//                           int>,  // whether is implemented
//                  void>
// f(const T& t) {}
// std::enable_if_t<std::is_void_v<ToInt_t<decltype(std::declval<T>().method())>>,
//                  void>
// f(const T& t) {}
// std::enable_if_t<std::is_void_v<std::void_t<
//                      decltype(std::declval<RemoveRef_t<T>>().method())>>,
//                  void>
// f(const T& t) {}
std::enable_if_t<
    std::is_void_v<std::void_t<
        decltype(std::declval<std::remove_reference_t<T>>().method())>>,
    void>
f(const T& t) {}

void Demo() {
    // Set<Test> set_test;
    // set_test.insert(const Test& item)

    Test1 t1;
    Test2 t2;
    Test3 t3;
    f<Test1>(t1);
    // f<Test2>(t2); // sf
    // f<Test3>(t3); // sf
}

int main(int argc, const char* argv[]) {
    Demo();

    return 0;
}