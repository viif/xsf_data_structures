#ifndef XSF_ARRAY_DEQUE_H
#define XSF_ARRAY_DEQUE_H

namespace xsf_data_structures {

// 使用环形数组实现的双端队列
template <typename T>
class XSFArrayDeque {
 public:
  XSFArrayDeque(size_t capacity = 2) { ReAlloc(capacity); }

  ~XSFArrayDeque() {
    Clear();
    // 避免调用T的析构函数
    ::operator delete(data_, capacity_ * sizeof(T));
  }

  // 增
  void PushFront(const T& value) {
    // 扩容
    if (size_ == capacity_) {
      ReAlloc(capacity_ * 2);
    }

    if (front_ == 0) {
      front_ = capacity_ - 1;
    } else {
      front_--;
    }

    data_[front_] = value;
    size_++;
  }

  void PushFront(T&& value) {
    // 扩容
    if (size_ == capacity_) {
      ReAlloc(capacity_ * 2);
    }

    if (front_ == 0) {
      front_ = capacity_ - 1;
    } else {
      front_--;
    }

    data_[front_] = std::move(value);
    size_++;
  }

  template <typename... Args>
  T& EmplaceFront(Args&&... args) {
    // 扩容
    if (size_ == capacity_) {
      ReAlloc(capacity_ * 2);
    }

    if (front_ == 0) {
      front_ = capacity_ - 1;
    } else {
      front_--;
    }

    new (&data_[front_]) T(std::forward<Args>(args)...);
    size_++;
    return data_[front_];
  }

  void PushBack(const T& value) {
    // 扩容
    if (size_ == capacity_) {
      ReAlloc(capacity_ * 2);
    }

    data_[rear_] = value;
    rear_++;
    if (rear_ == capacity_) {
      rear_ = 0;
    }
    size_++;
  }

  void PushBack(T&& value) {
    // 扩容
    if (size_ == capacity_) {
      ReAlloc(capacity_ * 2);
    }

    data_[rear_] = std::move(value);
    rear_++;
    if (rear_ == capacity_) {
      rear_ = 0;
    }
    size_++;
  }

  template <typename... Args>
  T& EmplaceBack(Args&&... args) {
    // 扩容
    if (size_ == capacity_) {
      ReAlloc(capacity_ * 2);
    }

    new (&data_[rear_]) T(std::forward<Args>(args)...);
    rear_++;
    if (rear_ == capacity_) {
      rear_ = 0;
    }
    size_++;
    return data_[rear_];
  }

  // 删
  void PopFront() {
    if (size_ == 0) {
      return;
    }

    // 缩容
    if (size_ < capacity_ / 4) {
      ReAlloc(capacity_ / 2);
    }

    data_[front_].~T();
    front_++;
    if (front_ == capacity_) {
      front_ = 0;
    }
    size_--;
  }

  void PopBack() {
    if (size_ == 0) {
      return;
    }

    // 缩容
    if (size_ < capacity_ / 4) {
      ReAlloc(capacity_ / 2);
    }

    if (rear_ == 0) {
      rear_ = capacity_ - 1;
    } else {
      rear_--;
    }

    data_[rear_].~T();
    size_--;
  }

  // 查、改
  T& Front() {
    if (size_ == 0) throw std::out_of_range("deque is empty");
    return data_[front_];
  }

  const T& Front() const {
    if (size_ == 0) throw std::out_of_range("deque is empty");
    return data_[front_];
  }

  T& Back() {
    if (size_ == 0) throw std::out_of_range("deque is empty");
    if (rear_ == 0) {
      return data_[capacity_ - 1];
    } else {
      return data_[rear_ - 1];
    }
  }

  const T& Back() const {
    if (size_ == 0) throw std::out_of_range("deque is empty");
    if (rear_ == 0) {
      return data_[capacity_ - 1];
    } else {
      return data_[rear_ - 1];
    }
  }

  // 工具函数
  bool Empty() const { return size_ == 0; }

  void Clear() {
    for (size_t i = 0; i < size_; i++) {
      data_[(front_ + i) & mask_].~T();
    }
    size_ = 0;
  }

 private:
  // 将输入的 n 转化为 2 的指数，比如输入 12，返回 16
  size_t CeilToPow2(size_t n) {
    // size_t 型最大值为 2^64 - 1
    // 所以无法向上取整到 2^64
    if (n > 0x8000000000000000) {
      return 0x8000000000000000;
    }

    // 位运算技巧，参考如下链接：
    // http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n |= n >> 32;
    n++;

    return n;
  }

  void ReAlloc(size_t new_capacity) {
    // 1. allocate a new block of memory
    // 2. move old elements into new block
    // 3. delete old block

    // 将 capacity 转化为 2 的指数
    new_capacity = CeilToPow2(new_capacity);

    // 不需要构造 T，只需要分配内存块
    T* new_block = (T*)::operator new(new_capacity * sizeof(T));

    //   first-----last
    // ---last    first---

    for (size_t i = 0; i < size_; i++) {
      // 将已有对象移动构造到新分配的内存块中
      new (&new_block[i]) T(std::move(data_[(front_ + i) & mask_]));
      // 析构旧内存块中的对象
      data_[(front_ + i) & mask_].~T();
    }

    // 重置 front_ 和 rear_
    front_ = 0;
    rear_ = size_;

    // 避免调用T的析构函数
    ::operator delete(data_, capacity_ * sizeof(T));

    data_ = new_block;
    capacity_ = new_capacity;

    // 若保证 capacity_ 为 2 的指数
    // 则 n % capacity_ 等价于 n & mask_
    mask_ = new_capacity - 1;
  }

  T* data_{nullptr};
  size_t size_{0};
  size_t capacity_{0};

  // data_ 的索引区间 [front_, rear_) 存储着添加的元素
  size_t front_{0};  // 头指针
  size_t rear_{0};  // 尾指针，若队列不空，指向队列尾元素的下一个位置

  size_t mask_{1};  // 用于防止索引越界
};

}  // namespace xsf_data_structures

#endif  // XSF_ARRAY_DEQUE_H