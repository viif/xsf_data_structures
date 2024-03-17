#ifndef XSF_ARRAY_LIST_H
#define XSF_ARRAY_LIST_H

#include <cstring>

namespace xsf_data_structures {

template <typename XSFArrayList>
class XSFArrayListIterator {
 public:
  using ValueType = typename XSFArrayList::ValueType;
  using PointerType = ValueType*;
  using ReferenceType = ValueType&;

  XSFArrayListIterator(PointerType ptr) : ptr_(ptr) {}

  XSFArrayListIterator& operator++() {
    ptr_++;
    return *this;
  }

  XSFArrayListIterator operator++(int) {
    XSFArrayListIterator it = *this;
    ++(*this);
    return it;
  }

  XSFArrayListIterator& operator--() {
    ptr_--;
    return *this;
  }

  XSFArrayListIterator operator--(int) {
    XSFArrayListIterator it = *this;
    --(*this);
    return it;
  }

  ReferenceType operator[](size_t index) { return *(ptr_ + index); }

  PointerType operator->() { return ptr_; }

  ReferenceType operator*() { return *ptr_; }

  bool operator==(const XSFArrayListIterator& other) const {
    return ptr_ == other.ptr_;
  }

  bool operator!=(const XSFArrayListIterator& other) const {
    return ptr_ != other.ptr_;
  }

 private:
  PointerType ptr_;
};

template <typename T>
class XSFArrayList {
 public:
  using ValueType = T;
  using Iterator = XSFArrayListIterator<XSFArrayList<T>>;

  XSFArrayList(size_t capacity = 2) { ReAlloc(capacity); }

  ~XSFArrayList() {
    Clear();
    // 避免调用T的析构函数
    ::operator delete(data_, capacity_ * sizeof(T));
  }

  // 增
  void PushBack(const T& value) {
    if (size_ >= capacity_) {
      // 扩容
      ReAlloc(capacity_ * 2);
    }
    data_[size_] = value;
    size_++;
  }

  void PushBack(T&& value) {
    if (size_ >= capacity_) {
      // 扩容
      ReAlloc(capacity_ * 2);
    }
    data_[size_] = std::move(value);
    size_++;
  }

  template <typename... Args>
  T& EmplaceBack(Args&&... args) {
    if (size_ >= capacity_) {
      // 扩容
      ReAlloc(capacity_ * 2);
    }
    // 原地构造的关键
    new (&data_[size_]) T(std::forward<Args>(args)...);
    size_++;
    return data_[size_];
  }

  void Insert(size_t index, const T& value) {
    CheckPosition(index);

    if (size_ >= capacity_) {
      // 扩容
      ReAlloc(capacity_ * 2);
    }
    // 搬移原数据
    memmove(data_ + index + 1, data_ + index, (size_ - index) * sizeof(T));
    // 插入新数据
    data_[index] = value;
    size_++;
  }

  void Insert(size_t index, T&& value) {
    CheckPosition(index);

    if (size_ >= capacity_) {
      // 扩容
      ReAlloc(capacity_ * 2);
    }
    // 搬移原数据
    memmove(data_ + index + 1, data_ + index, (size_ - index) * sizeof(T));
    // 插入新数据
    data_[index] = std::move(value);
    size_++;
  }

  // 删
  void PopBack() {
    if (Empty()) {
      return;
    }
    if (size_ <= capacity_ / 4) {
      // 缩容
      ReAlloc(capacity_ / 2);
    }
    // 删除数据
    size_--;
    data_[size_].~T();
  }

  void Erase(size_t index) {
    CheckElement(index);
    if (size_ <= capacity_ / 4) {
      // 缩容
      ReAlloc(capacity_ / 2);
    }
    // 搬移原数据
    memmove(data_ + index, data_ + index + 1, (size_ - index - 1) * sizeof(T));
    // 删除数据
    size_--;
    data_[size_].~T();
  }

  void Clear() {
    for (size_t i = 0; i < size_; i++) {
      data_[i].~T();
    }
    size_ = 0;
  }

  // 查、改
  T& operator[](size_t index) {
    CheckElement(index);
    return data_[index];
  }

  const T& operator[](size_t index) const {
    CheckElement(index);
    return data_[index];
  }

  Iterator begin() { return Iterator(data_); }

  Iterator end() { return Iterator(data_ + size_); }

  T& Front() { return *begin(); }

  const T& Front() const { return *begin(); }

  T& Back() { return *--end(); }

  const T& Back() const { return *--end(); }

  // 工具函数
  bool Empty() const { return size_ == 0; }

  size_t Size() const { return size_; }

 private:
  bool IsElementValid(size_t index) const { return index < size_; }

  bool IsPositionValid(size_t index) const { return index <= size_; }

  // 检查 index 索引位置是否可以存在元素
  void CheckElement(size_t index) const {
    if (!IsElementValid(index)) {
      throw std::out_of_range("Index out of range");
    }
  }

  // 检查 index 索引位置是否可以添加元素
  void CheckPosition(size_t index) const {
    if (!IsPositionValid(index)) {
      throw std::out_of_range("Index out of range");
    }
  }

  void ReAlloc(size_t new_capacity) {
    // 1. allocate a new block of memory
    // 2. move old elements into new block
    // 3. delete old block

    // 不需要构造 T，只需要分配内存块
    T* new_block = (T*)::operator new(new_capacity * sizeof(T));

    for (size_t i = 0; i < size_; i++) {
      // 将已有对象移动构造到新分配的内存块中
      new (&new_block[i]) T(std::move(data_[i]));
      // 析构旧内存块中的对象
      data_[i].~T();
    }

    // 避免调用T的析构函数
    ::operator delete(data_, capacity_ * sizeof(T));

    data_ = new_block;
    capacity_ = new_capacity;
  }

  T* data_{nullptr};
  size_t size_{0};
  size_t capacity_{0};
};

}  // namespace xsf_data_structures

#endif  // XSF_ARRAY_LIST_H