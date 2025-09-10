#include <iostream>

struct Point {
  double x, y;
};

struct Compare {
  auto operator()(const Point& l, const Point& r) const noexcept -> bool;
};

auto Compare::operator()(const Point& l, const Point& r) const noexcept
    -> bool {
  return l.x < r.x && l.y < r.y;
}

template <typename T>
struct Less {
  auto operator()(const T& l, const T& r) const noexcept -> bool {
    return l < r;
  }
};

template <typename T>
struct Greater {
  auto operator()(const T& l, const T& r) const noexcept -> bool {
    return l > r;
  }
};

// template <typename T, template <typename> typename Compare>
// void Sort(T* arr, size_t size) {
template <typename T>
void Sort(T* arr, size_t size, bool (*compare)(const T&, const T&)) {
  for (int i = size; i > 1; i--) {
    for (int j = 1; j < i; j++) {
      if (compare(arr[j], arr[j - 1])) {
        T tmp = arr[j];
        arr[j] = arr[j - 1];
        arr[j - 1] = tmp;
      }
    }
  }
}

void Demo() {
  Point p1(2, 5);
  Point p2(1, 1);
  Compare com;
  std::cout << com(p1, p2) << '\n';

  int arr[]{3, 5, 1, 4, 2};
  // Sort<int, Greater>(arr, 5);
  // Sort<int>(arr, 5, [](const int& l, const int& r) -> bool { return l < r;
  // });
  auto less_int = [](const int& l, const int& r) -> bool {
    return l < r;
  };  // the lambda expr will only implicitly transform to function pointer when
      // capture list is empty
  Sort<int>(arr, 5, less_int);
  for (int item : arr) {
    std::cout << item << ", ";
  }
}

auto main(int argc, const char* argv[]) -> int {
  Demo();

  return 0;
}