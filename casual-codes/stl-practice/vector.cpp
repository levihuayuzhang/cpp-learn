// #include <array>
// #include <vector>
#include <cstddef>
#include <cstdlib>
#include <memory>
#include <type_traits>
#include <utility>

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

  class Iterator {
   public:
    auto operator*() -> T&;
    auto operator->() -> T*;
    auto operator++() -> void;
    auto operator--() -> void;
    auto operator[](intptr_t offset) -> Iterator;
    auto operator==(const Iterator& iter) -> void;
    auto operator!=(const Iterator& iter) -> void;

   private:
    // auto GetValue() -> T&;
    // auto MoveBack(size_t offset) -> void;
    // auto MoveFront(size_t offset) -> void;

    T* data_;
    Vector* vec_;
    friend class Vector;
  };

  friend class Iterator;

  Vector();
  ~Vector();

  Vector(const Vector& other);
  Vector(Vector&& other) noexcept;

  auto operator=(const Vector& other) -> Vector&;
  auto operator=(Vector&& other) -> Vector&;

  auto Size() const -> size_t;
  auto Capacity() const -> size_t;
  auto Empty() const -> bool;

  // auto PushBack(const T& val) -> void;
  template <typename... Args>
  auto PushBack(Args&&... args) -> void;

  auto PopBack() -> void;

  auto At(size_t index) -> T&;
  [[nodiscard]] auto At(size_t index) const -> const T&;

  auto operator[](size_t index) -> T&;
  auto operator[](size_t index) const -> const T&;

  auto Find(const T& t) -> Iterator;
  [[nodiscard]] auto Find(const T& t) const -> const T*;
  auto Erase(Iterator iter) -> Iterator;

  auto begin() -> Iterator;
  auto end() -> Iterator;
  [[nodiscard]] auto cbegin() const -> const T*;
  [[nodiscard]] auto cend() const -> const T*;
  [[nodiscard]] auto begin() const -> const T*;
  [[nodiscard]] auto end() const -> const T*;

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
Vector<T>::Vector(Vector&& other) noexcept : capacity_(other.capacity_), size_(other.size_), data_(other.data_) {
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
    new (new_data + size_) T(std::forward<Args>(args)...);
    for (size_t i = 0; i < size_; i++) {
      if constexpr (std::is_nothrow_move_constructible_v<T>) {
        new (new_data + i) T(std::move(data_[i]));
      } else {
        new (new_data + i) T(data_[i]);
      }
    }
    for (size_t i = 0; i < size_; i++) {
      data_[i].~T();
    }

    operator delete(data_);
    data_ = new_data;
  } else {
    new (data_ + size_) T(std::forward<Args>(args)...);
  }

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
auto Vector<T>::Find(const T& t) -> Vector<T>::Iterator {
  for (T* tmp = data_; tmp < data_ + size_; tmp++) {
    if (tmp == t) {
      Iterator iter;
      iter.vec_ = this;
      iter.data_ = tmp;
      return iter;
    }
  }

  return this->end();
}

template <typename T>
auto Vector<T>::Find(const T& t) const -> const T* {
  for (const T* tmp = data_; tmp < data_ + size_; tmp++) {
    if (tmp == t) {
      return tmp;
    }
  }

  return nullptr;
}

template <typename T>
auto Vector<T>::Erase(Vector<T>::Iterator iter) -> Vector<T>::Iterator {
  if (iter.vec_ != this || iter.data_ - data_ < 0 || iter.data_ - data_ >= size_) {
    throw 4;
  }
  for (T* tmp = iter.data_ + 1; tmp < data_ + size_; tmp++) {
    if constexpr (std::is_nothrow_move_assignable_v<T>) {
      tmp[-1] = std::move(tmp);
    } else {
      tmp[-1] = tmp;
    }
  }
  size_--;
  return iter;
}

template <typename T>
auto Vector<T>::begin() -> Vector<T>::Iterator {
  Iterator iter;
  iter.data_ = data_;
  return iter;
}

template <typename T>
auto Vector<T>::end() -> Vector<T>::Iterator {
  return Iterator();
}

template <typename T>
auto Vector<T>::cbegin() const -> const T* {
  return data_;
}

template <typename T>
auto Vector<T>::cend() const -> const T* {
  return data_ + size_;
}

template <typename T>
auto Vector<T>::begin() const -> const T* {
  return this->cbegin();
}

template <typename T>
auto Vector<T>::end() const -> const T* {
  return this->cend();
}

template <typename T>
auto Vector<T>::Iterator::operator*() -> T& {
  return *data_;
}

template <typename T>
auto Vector<T>::Iterator::operator->() -> T* {
  return data_;
}

template <typename T>
auto Vector<T>::Iterator::operator++() -> void {
  if (data_ - vec_->data_ >= vec_->size_) {
    data_ = nullptr;
  } else {
    data_++;
  }
}

template <typename T>
auto Vector<T>::Iterator::operator--() -> void {
  if (data_ <= vec_->data_) {
    data_ = nullptr;
  } else {
    data_--;
  }
}

template <typename T>
auto Vector<T>::Iterator::operator[](intptr_t offset) -> Vector<T>::Iterator {
  if (data_ + offset < vec_->data_ || data_ + offset >= vec_->data_ + vec_->size_) {
    Iterator iter;
    iter.vec_ = vec_;
    iter.data_ = nullptr;
    return iter;
  }

  Iterator iter;
  iter.data_ = data_ + offset;
  iter.vec_ = vec_;
  return iter;
}

template <typename T>
auto Vector<T>::Iterator::operator==(const Iterator& iter) -> void {
  return data_ == iter.data_ && vec_ == iter.vec_;
}

template <typename T>
auto Vector<T>::Iterator::operator!=(const Iterator& iter) -> void {
  // return data_ != iter.data_ || vec_ != iter.vec_;
  return !this->operator==(iter);
}

// template <typename T>
// auto Vector<T>::Iterator::GetValue() -> T& {}

// template <typename T>
// auto Vector<T>::Iterator::MoveBack(size_t offset) -> void {}

// template <typename T>
// auto Vector<T>::Iterator::MoveFront(size_t offset) -> void {}

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
  // for (auto& item : ve) {
  // }

  return 0;
}