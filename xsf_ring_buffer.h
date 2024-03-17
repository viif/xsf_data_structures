#ifndef XSF_RING_BUFFER_H
#define XSF_RING_BUFFER_H

#include <cstring>

namespace xsf_data_structures {

class XSFRingBuffer {
 public:
  XSFRingBuffer(size_t capacity = 1024) { ReAlloc(capacity); }

  ~XSFRingBuffer() { delete[] buffer_; }

  // 从 RingBuffer 中读取元素到 out 中，返回读取的字节数
  size_t Read(char* out, size_t out_size) {
    if (out == nullptr || size_ == 0 || Empty() || out_size == 0) {
      return 0;
    }
    size_t n = (out_size < size_) ? out_size : size_;
    if (r_ < w_) {
      // 情况1：r---w
      memcpy(out, buffer_ + r_, n);
    } else {
      // 情况2：---w  r---
      if (r_ + n <= capacity_) {
        // 情况2.1：---w  r--- 读取后变成 ---w  **r-
        memcpy(out, buffer_ + r_, n);
      } else {
        // 情况2.2：---w  r--- 读取后变成 *r--w  ***
        size_t m = capacity_ - r_;
        memcpy(out, buffer_ + r_, m);
        memcpy(out + m, buffer_, n - m);
      }
    }
    // 更新读指针
    r_ = (r_ + n) & mask_;
    // 更新可读字节数
    size_ -= n;
    return n;
  }

  // 将 in 中的数据写入 RingBuffer，返回写入字节的个数
  size_t Write(char* in, size_t in_size) {
    if (in == nullptr || in_size == 0) {
      return 0;
    }
    // 计算空闲空间大小
    size_t space = capacity_ - size_;
    if (in_size > space) {
      // 扩容
      ReAlloc(capacity_ + in_size);
    }

    if (r_ < w_) {
      // 情况1：r_---w_
      if (capacity_ - w_ >= in_size) {
        // 情况1.1：r---w 写入后变成 r---**w
        memcpy(buffer_ + w_, in, in_size);
      } else {
        // 情况1.2：r---w 写入后变成 **w  r---*
        size_t n = capacity_ - w_;
        memcpy(buffer_ + w_, in, n);
        memcpy(buffer_, in + n, in_size - n);
      }
    } else {
      // 情况2：---w_    r_---
      memcpy(buffer_ + w_, in, in_size);
    }
    // 更新写指针
    w_ = (w_ + in_size) & mask_;
    // 更新可读字节数
    size_ += in_size;
    return in_size;
  }

  // 返回可读的字节数量
  size_t Length() const { return size_; }

  // 是否没有可读的数据
  bool Empty() const { return size_ == 0; }

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

    // 分配新内存块
    char* new_block{new char[new_capacity]};

    // 将已有数据移动到新分配的内存块中
    if (r_ < w_) {
      //   r_-----w_
      memcpy(new_block, buffer_ + r_, size_);
    } else {
      // ---w_    r_---
      memcpy(new_block, buffer_ + r_, capacity_ - r_);
      memcpy(new_block + capacity_ - r_, buffer_, w_);
    }

    // 重置 r_ 和 w_
    r_ = 0;
    w_ = size_;

    // 释放旧内存块
    delete[] buffer_;

    buffer_ = new_block;
    capacity_ = new_capacity;

    // 若保证 capacity_ 为 2 的指数
    // 则 n % capacity_ 等价于 n & mask_
    mask_ = new_capacity - 1;
  }

  char* buffer_{nullptr};
  size_t size_{0};
  size_t capacity_{0};

  size_t r_{0};     // 读指针
  size_t w_{0};     // 写指针
  size_t mask_{0};  // 用于防止索引越界
};

}  // namespace xsf_data_structures

#endif  // XSF_RING_BUFFER_H