#include <array>
#include <cstdint>
#include <iostream>

// std::array<uint8_t, 2048> buf;
uint8_t buf[2048];
uint8_t* pos = buf;

auto Alloc(size_t size) -> void* {
  auto tmp = pos;
  pos += size;
  return tmp;
}

class Point {
 public:
  Point(double x, double y);
  auto Show() const -> void;

 private:
  double x_, y_;
};

struct Test {
  // Test(Point& p);
  Test(Point&& p);
};

Point::Point(double x, double y) : x_(x), y_(y) {}
auto Point::Show() const -> void {
  std::cout << x_ << ", " << y_ << '\n';
}

class Circle {
 public:
  Circle(const Point& center, double r);
};

// except for rval ref (nomal lval) -> T&
// rval -> T&&
// rval ref -> T&&
template <typename T>
auto Forward(T& t) -> T&& {
  return static_cast<T&&>(t);
}
template <typename T>
auto Forward(T&& t) -> T&& {
  return static_cast<T&&>(t);
}

// reference collapsing
template <typename T, typename... Args>
auto Create(Args&&... args) -> T* {
  T* buf = static_cast<T*>(Alloc(sizeof(T)));
  // return new (buf) T(Forward<Args>(args)...);
  return new (buf) T(std::forward<Args>(args)...);
}

auto Demo() -> void {
  // perfect forwaring
  auto&& res1 = Forward<int>(5);     // int&&
  int a = 5;                         // lval
  auto&& res2 = Forward<int&>(a);    // int&
  int&& rr = std::move(a);           // rval ref (which is lval)
  auto&& res3 = Forward<int&&>(rr);  // int&&

  // int* data = static_cast<int*>(Alloc(sizeof(int)));
  // *data = 18;
  int* data = Create<int, int>(18);

  // Point* p = static_cast<Point*>(Alloc(sizeof(Point)));
  // new (p) Point(1, 4.6);
  // p->Show();
  Point* p = Create<Point, double, double>(1, 5.6);
  p->Show();

  // Circle* c = Create<Circle, Point, double>(*p, 2.3);

  Point p2(1, 2);
  Point&& rp = std::move(p2);
  // Test* t = Create<Test, Point&>(p2);
  // Test* t = Create<Test, Point&&>(p2);
}

auto main(int argc, const char* argv[]) -> int {
  Demo();

  return 0;
}