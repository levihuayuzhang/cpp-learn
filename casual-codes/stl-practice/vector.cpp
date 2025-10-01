#include <array>
#include <cstddef>
#include <memory>
#include <type_traits>
#include <vector>

// linear table, sequential storage
// consencutive memory space -> scaling
// random access, apend, delete
// all type support

namespace demo {

template <typename T>
class Vector {
 public:
  using ValueType = T;
  using Pointer = T*;

  Vector();
  ~Vector();

  Vector(const Vector&);
  Vector(Vector&&);

  auto operator=(const Vector&) -> Vector&;
  auto operator=(Vector&& other) -> Vector&;

  auto Size() const -> size_t;
  auto Capacity() const -> size_t;
  auto Empty() const -> bool;

  // auto PushBack(const T& val) -> void;
  template <typename... Args>
  auto PushBack(Args&&... args) -> void;

  auto PopBack() -> void;

  auto At(size_t index) -> T&;
  auto At(size_t index) const -> const T&;

  auto operator[](size_t index) -> T&;
  auto operator[](size_t index) const -> const T&;

  auto Find(const T& t) -> T*;
  auto Find(const T& t) const -> const T*;
  auto Erase(T* t) -> T*;

  auto begin() -> T*;
  auto end() -> T*;
  auto cbegin() const -> const T*;
  auto cend() const -> const T*;
  auto begin() const -> const T*;
  auto end() const -> const T*;

 private:
  size_t capacity_;
  T* data_;
  size_t size_;
};

template <typename T>
Vector<T>::Vector() : capacity_(2), data_(static_cast<Pointer>(operator new(sizeof(T) * capacity_))), size_(0) {}

template <typename T>
Vector<T>::~Vector() {
  for (size_t i = 0; i < size_; i++) {
    data_[i].~T();
  }

  operator delete(data_);
}

template <typename T>
Vector<T>::Vector(const Vector& other)
    : capacity_(other.capacity_),
      size_(other.size_),
      data_(static_cast<Pointer>(operator new(sizeof(T) * other.capacity_))) {
  for (size_t i = 0; i < size_; i++) {
    new (data_ + i) T(other.data_[i]);
  }
}

template <typename T>
Vector<T>::Vector(Vector&& other) : capacity_(other.capacity_), size_(other.size_), data_(other.data_) {
  other.capacity_ = 0;
  other.size_ = 0;
  other.data_ = nullptr;
}

template <typename T>
auto Vector<T>::operator=(const Vector& other) -> Vector<T>& {
  if (this != std::addressof(other)) {
    for (size_t i = 0; i < size_; i++) {
      data_[i].~T();
    }

    if (capacity_ >= other.size_) {
      operator delete(data_);
      data_ = operator new(sizeof(T) * other.size_);
      capacity_ = other.size_;
    }

    for (size_t i = 0; i < size_; i++) {
      new (data_ + i) T(other.data_[i]);
    }
  }

  return *this;
}

template <typename T>
auto Vector<T>::operator=(Vector&& other) -> Vector<T>& {
  if (this != std::addressof(other)) {
    for (size_t i = 0; i < size_; i++) {
      data_[i].~T();
    }
    size_ = other.size_;
    capacity_ = other.capacity_;
    operator delete(data_);
    data_ = other.data_;
    other.capacity_ = 0;
    other.size_ = 0;
    other.data_ = nullptr;
  }

  return *this;
}

template <typename T>
auto Vector<T>::Size() const -> size_t {
  return size_;
}

template <typename T>
auto Vector<T>::Capacity() const -> size_t {
  return capacity_;
}

// template <typename T>
// auto Vector<T>::PushBack(const T& val) -> void {
template <typename T>
template <typename... Args>
auto Vector<T>::PushBack(Args&&... args) -> void {
  if (capacity_ < size_ + 1) {
    if (capacity_ == 0) {
      capacity_ = 2;
    } else {
      capacity_ *= 2;
    }

    T* new_data = static_cast<T*>(operator new(sizeof(T) * capacity_));
    for (size_t i = 0; i < size_; i++) {
      if constexpr (std::is_nothrow_move_constructible_v<T>) {
        new (new_data + i) T(std::move(data_[i]));
      } else {
        new (new_data + i) T(data_[i]);
      }
    }
    if constexpr (std::is_nothrow_move_constructible_v<T>) {
      for (size_t i = 0; i < size_; i++) {
        data_[i].~T();
      }
    }

    operator delete(data_);
    data_ = new_data;
  }

  new (data_ + size_) T(std::forward<Args>(args)...);
  size_++;
}

template <typename T>
auto Vector<T>::PopBack() -> void {
  if (this->Empty()) {
    throw 1;
  }
  data_[size_ - 1].~T();
  size_--;
}

template <typename T>
auto Vector<T>::Empty() const -> bool {
  return size_ == 0;
}

template <typename T>
auto Vector<T>::At(size_t index) -> T& {
  if (index >= size_) {
    throw 2;
  }

  return data_[index];
}

template <typename T>
auto Vector<T>::At(size_t index) const -> const T& {
  if (index >= size_) {
    throw 2;
  }

  return data_[index];
}

template <typename T>
auto Vector<T>::operator[](size_t index) -> T& {
  return this->At(index);
}

template <typename T>
auto Vector<T>::operator[](size_t index) const -> const T& {
  return this->At(index);
}

template <typename T>
auto Vector<T>::Find(const T& t) -> T* {}

template <typename T>
auto Vector<T>::Find(const T& t) const -> const T* {}

template <typename T>
auto Vector<T>::Erase(T* t) -> T* {}

template <typename T>
auto Vector<T>::begin() -> T* {}

template <typename T>
auto Vector<T>::end() -> T* {}

template <typename T>
auto Vector<T>::cbegin() const -> const T* {}

template <typename T>
auto Vector<T>::cend() const -> const T* {}

template <typename T>
auto Vector<T>::begin() const -> const T* {}

template <typename T>
auto Vector<T>::end() const -> const T* {}

}  // namespace demo

struct Test {
  Test(int a);
  Test(const Test&) = delete;
  Test(Test&&) noexcept {}
};

auto main(int argc, const char* argv[]) -> int {
  // int arr[] = {1, 2, 3};
  //   std::array<int, 3> arr2 = {4, 5, 6};
  // std::vector<int> ve;

  demo::Vector<Test> ve;

  return 0;
}