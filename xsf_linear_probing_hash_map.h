#ifndef XSF_LINEAR_PROBING_HASH_MAP_H
#define XSF_LINEAR_PROBING_HASH_MAP_H

namespace xsf_data_structures {

template <typename K, typename V, class Hash>
class XSFLinearProbingHashMap {
 private:
  // 键值对节点
  enum NodeType { ACTIVE, EMPTY, DELETED };
  struct Node {
    K key{};
    V value{};
    NodeType type{EMPTY};

    Node() = default;
  };

 public:
  XSFLinearProbingHashMap(size_t capacity = 4)
      : capacity_(CeilToPow2(capacity)),
        mask_(capacity_ - 1),
        table_(new Node[capacity_]) {}

  ~XSFLinearProbingHashMap() { delete[] table_; }

  // 增、改
  V &operator[](const K &key) {
    if (size_ >= capacity_ / 2) {
      Resize(capacity_ * 2);
    }
    size_t index;
    if (LinearProbing(key, index)) {
      // key 已存在
      return table_[index].value;
    } else {
      // key 不存在，搜索空槽位或者已删除槽位
      size_t i = HashIndex(key);
      while (table_[i].type == ACTIVE) {
        i = (i + 1) & mask_;
      }
      table_[i].key = key;
      table_[i].type = ACTIVE;
      size_++;
      return table_[i].value;
    }
  }

  V &operator[](K &&key) {
    if (size_ >= capacity_ / 2) {
      Resize(capacity_ * 2);
    }
    size_t index;
    if (LinearProbing(std::move(key), index)) {
      // key 已存在
      return table_[index].value;
    } else {
      // key 不存在，搜索空槽位或者已删除槽位
      size_t i = HashIndex(std::move(key));
      while (table_[i].type == ACTIVE) {
        i = (i + 1) & mask_;
      }
      table_[i].key = std::move(key);
      table_[i].type = ACTIVE;
      size_++;
      return table_[i].value;
    }
  }

  bool Contains(const K &key) {
    size_t index;
    return LinearProbing(key, index);
  }

  bool Contains(K &&key) {
    size_t index;
    return LinearProbing(std::move(key), index);
  }

  // 删，使用节点标记保持连续性
  size_t Erase(const K &key) {
    size_t index;
    if (LinearProbing(key, index)) {
      table_[index].type = DELETED;
      size_--;
      return 1;
    }
    return 0;
  }

  size_t Erase(K &&key) {
    size_t index;
    if (LinearProbing(std::move(key), index)) {
      table_[index].type = DELETED;
      size_--;
      return 1;
    }
    return 0;
  }

  void Clear() {
    for (size_t i = 0; i < capacity_; i++) {
      if (table_[i].type != EMPTY) {
        table_[i].type = EMPTY;
      }
    }
    size_ = 0;
  }

  // 工具函数
  size_t Size() const { return size_; }

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

  // 增加 table_ 的大小
  void Resize(size_t new_capacity) {
    // 1. allocate a new block of memory
    // 2. move old elements into new block
    // 3. delete old block

    // 将 capacity 转化为 2 的指数
    new_capacity = CeilToPow2(new_capacity);

    // 分配内存块并构造 Node
    Node *new_table = new Node[new_capacity];

    // 若保证 capacity_ 为 2 的指数
    // 则 n % capacity_ 等价于 n & mask_
    mask_ = new_capacity - 1;

    for (size_t i = 0; i < capacity_; i++) {
      if (table_[i].type == ACTIVE) {
        // 将旧 table_ 中的元素重新哈希到新 table_ 中
        size_t index = HashIndex(table_[i].key);
        while (new_table[index].type != EMPTY) {
          index = (index + 1) & mask_;
        }
        new_table[index].key = std::move(table_[i].key);
        new_table[index].value = std::move(table_[i].value);
        new_table[index].type = ACTIVE;
      }
    }

    // 释放旧内存块
    delete[] table_;

    table_ = new_table;
    capacity_ = new_capacity;
  }

  // 对 key 进行线性探查
  bool LinearProbing(const K &key, size_t &index) {
    size_t i{HashIndex(key)};
    size_t step{0};
    for (; table_[i].type != EMPTY; i = (i + 1) & mask_, step++) {
      // 跳过已删除的节点
      if (table_[i].type == DELETED) {
        continue;
      }
      // 找到 key
      if (table_[i].key == key) {
        index = i;
        return true;
      }
      // 防止死循环
      if (step > capacity_) {
        Resize(capacity_);
        // 调整后重新探测
        LinearProbing(key, index);
        return false;
      }
    }
    index = i;
    return false;
  }

  bool LinearProbing(K &&key, size_t &index) {
    size_t i{HashIndex(std::move(key))};
    size_t step{0};
    for (; table_[i].type != EMPTY; i = (i + 1) & mask_, step++) {
      // 跳过已删除的节点
      if (table_[i].type == DELETED) {
        continue;
      }
      // 找到 key
      if (table_[i].key == key) {
        index = i;
        return true;
      }
      // 防止死循环
      if (step > capacity_) {
        Resize(capacity_);
        // 调整后重新探测
        LinearProbing(std::move(key), index);
        return false;
      }
    }
    index = i;
    return false;
  }

  // 哈希函数，将键映射到 table 的索引
  size_t HashIndex(const K &key) { return hash_(key) & mask_; }

  size_t HashIndex(K &&key) { return hash_(std::move(key)) & mask_; }

  Hash hash_{};

  size_t size_{0};
  size_t capacity_{4};
  size_t mask_{capacity_ - 1};

  Node *table_{nullptr};
};

}  // namespace xsf_data_structures

#endif  // XSF_LINEAR_PROBING_HASH_MAP_H