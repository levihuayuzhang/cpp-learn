#include <iostream>
#include <string>

// template <typename T, size_t N>
// class Array {
//    public:
//     Array() = default;
//     Array(int n, T arg);

//     auto at(int i) -> T&;
//     auto operator[](int i) -> T&;

//    private:
//     T data[N];
// };

// template <typename T, size_t N>
// Array<T, N>::Array(int n, T arg) {
//     for (int i = 0; i < n; i++) {
//         data[i] = arg;
//     }
// }

// template <typename T, size_t N>
// auto Array<T, N>::at(int i) -> T& {
//     return data[i];
// }

// template <typename T, size_t N>
// auto Array<T, N>::operator[](int i) -> T& {
//     return this->at(i);
// }

template <typename T>
struct ArgType {
    using type = const T&;
};

template <>
struct ArgType<int> {
    using type = int;
};
template <>
struct ArgType<double> {
    using type = double;
};
template <>
struct ArgType<char> {
    using type = char;
};

// partial specialization
template <typename T>
struct ArgType<T*> {
    using type = T*;
};

template <typename T>
using ArgType_t = typename ArgType<T>::type;

template <typename T>
void Show(ArgType_t<T> t) {
    std::cout << t << '\n';
}

auto Demo() -> void {
    // Array<int, 10> arr(3, 8);
    // std::cout << arr[2] << '\n';
    // arr[8] = 10;
    // std::cout << arr[8] << '\n';

    Show<int>(3);
    std::string str = "123";
    Show<std::string>(str);

    std::string* str_p = &str;
    Show<std::string*>(str_p);
}

auto main(int argc, const char* argv[]) -> int {
    Demo();

    return 0;
}
