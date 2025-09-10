#include <functional>
#include <iostream>
#include <unordered_set>
#include <vector>

template <typename T, typename Compare>
auto IsMatch(const std::vector<T>& ve, const Compare& com = Compare{}) -> bool {
  for (const auto& v : ve) {
    if (com(v)) {
      return true;
    }
  }
  return false;
}

// template <typename T>
// struct CompareFlag {
//   auto operator()(const T& item) const -> bool { return item < val_; }
//   explicit CompareFlag(const T& val) : val_(val) {}

//  private:
//   const T& val_;
// };

struct Point {
  int x, y;

  auto operator==(const Point& op) const -> bool {
    return x == op.x && y == op.y;
  }
};

struct HashPoint {
  auto operator()(const Point& p) const -> size_t {
    return std::hash<int>()(p.x + p.y);
  }
};

template <typename T>
auto Sort(T* arr, size_t size, bool (*less)(const T&, const T&)) -> void {
  for (size_t i = size - 1; i > 1; i--) {
    for (int j = 0; j < i; j++) {
      if (less(arr[j + 1], arr[j])) {
        std::swap(arr[j], arr[j + 1]);
      }
    }
  }
}

template <typename T>
// auto FindIf(T* arr, size_t size, auto (*is_match)(const T&)->bool) -> T* {
auto FindIf(T* arr, size_t size, std::function<auto(const T&)->bool> is_match)
    -> T* {
  for (T* item = arr; item < arr + size; item++) {
    if (is_match(*item)) {
      return item;
    }
  }
  return nullptr;
}

struct __an {
  __an(int flag) : flag_(flag) {}
  auto operator()(const int& item) const -> bool { return item < 0; }

 private:
  int flag_;
};

struct Test {
  int a, b;

  auto f() -> void;
  auto f2() -> void;
};

auto Test::f() -> void {
  int arr[]{1, 3, 2, 5, 4};
  auto p = FindIf<int>(arr, sizeof(arr),
                       // [a = this->a](const int& item) -> bool { return item <
                       // a; }); [this](const int& item) -> bool { return item <
                       // this->a; }); [this](const int& item) -> bool {
                       [&, this](const int& item) -> bool {
                         // this->f2();
                         f2();
                         return item < a;
                       });
}

void Demo(int flag) {
  std::vector<int> ve{1, 3, -1, -9, 0};

  // CompareFlag cf(flag);
  // bool is_match = IsMatch<int, CompareFlag<int>>(ve, cf);
  auto iter = std::find_if(ve.begin(), ve.end(),
                           [flag](int item) -> bool { return item < flag; });

  Point p1{1, 2};
  Point p2{3, 3};

  // auto func = [](const Point& p) -> size_t {
  //   return std::hash<int>()(p.x + p.y);
  // };

  std::unordered_set<Point, decltype([](const Point& p) -> size_t {
                       return std::hash<int>()(p.x + p.y);
                     })>
      set_p;
  set_p.insert(p1);
  set_p.insert(p2);

  int arr[]{1, 5, -3, 4, 2};
  auto p = FindIf<int>(arr, sizeof(arr),
                       [](const int& item) -> bool { return item < 0; });
  if (p != nullptr) {
    std::cout << *p << '\n';
  }

  auto is_match =
      [flag](const int& item) -> bool {  // var in capture list is a member var
    return item < flag;
  };  // lambda: anonymous imitation function
  auto p3 = FindIf<int>(arr, sizeof(arr),
                        is_match);  // not compatible with function pointer (use
                                    // function object instead)

  {
    int a = 5;
    double d = 9.6;
    // auto f = [=]() -> void { std::cout << a << ", " << d << '\n'; }; // = for
    // all var in range
    auto f = [&]() -> void {  // & for all ref of var in range
      a = 8;
      std::cout << a << ", " << d << '\n';
    };

    f();
    std::cout << a << ", " << d << '\n';
  }
}

auto main(int argc, const char* argv[]) -> int {
  Demo(5);

  return 0;
}
