#ifndef XSF_SEPARATE_CHAINING_HASH_MAP_H
#define XSF_SEPARATE_CHAINING_HASH_MAP_H

namespace xsf_data_structures {

template <typename K, typename V, class Hash>
class XSFSeparateChainingHashMap;

template <typename K, typename V, class Hash>
class Slot {
 private:
  // 单链表节点
  struct Node {
    K key{};
    V value{};
    Node* next{nullptr};

    Node(const K& k, const V& v, Node* n) : key(k), value(v), next(n) {}
    Node(const K& k, V&& v, Node* n) : key(k), value(std::move(v)), next(n) {}
    Node(K&& k, const V& v, Node* n) : key(std::move(k)), value(v), next(n) {}
    Node(K&& k, V&& v, Node* n)
        : key(std::move(k)), value(std::move(v)), next(n) {}
  };

 public:
  Slot() {
    head_ = new Node(K{}, V{}, nullptr);
    tail_ = new Node(K{}, V{}, nullptr);
    head_->next = tail_;
  }

  ~Slot() {
    Clear();
    delete head_;
    delete tail_;
  }

  // 增、改
  V& operator[](const K& key) {
    Node* node = FindNode(key);
    if (node != nullptr) {
      return node->value;
    } else {
      return InsertNode(key);
    }
  }

  V& operator[](K&& key) {
    Node* node = FindNode(std::move(key));
    if (node != nullptr) {
      return node->value;
    } else {
      return InsertNode(std::move(key));
    }
  }

  // 删
  size_t Erase(const K& key) {
    size_t count{0};
    for (Node *prev = head_, *curr = head_->next; curr != tail_;
         prev = curr, curr = curr->next) {
      if (curr->key == key) {
        prev->next = curr->next;
        delete curr;
        curr = prev;
        count++;
      }
    }
    size_ -= count;
    return count;
  }

  size_t Erase(K&& key) {
    size_t count{0};
    for (Node *prev = head_, *curr = head_->next; curr != tail_;
         prev = curr, curr = curr->next) {
      if (curr->key == key) {
        prev->next = curr->next;
        delete curr;
        curr = prev;
        count++;
      }
    }
    size_ -= count;
    return count;
  }

  void Clear() {
    while (head_->next != tail_) {
      Node* p = head_->next;
      head_->next = p->next;
      delete p;
    }
    size_ = 0;
  }

  // 查
  bool Contains(const K& key) { return FindNode(key) != nullptr; }

  bool Contains(K&& key) { return FindNode(std::move(key)) != nullptr; }

  size_t Count(const K& key) {
    size_t count{0};
    for (Node* p = head_; p != tail_; p = p->next) {
      if (p->key == key) {
        count++;
      }
    }
    return count;
  }

  size_t Count(K&& key) {
    size_t count{0};
    for (Node* p = head_; p != tail_; p = p->next) {
      if (p->key == key) {
        count++;
      }
    }
    return count;
  }

  // 工具函数
  size_t Size() const { return size_; }

  bool Empty() const { return size_ == 0; }

 private:
  // 找到 key 对应的单链表节点
  Node* FindNode(const K& key) {
    // 遍历单链表
    for (Node* p = head_; p != tail_; p = p->next) {
      if (p->key == key) {
        return p;
      }
    }
    return nullptr;
  }

  Node* FindNode(K&& key) {
    // 遍历单链表
    for (Node* p = head_; p != tail_; p = p->next) {
      if (p->key == key) {
        return p;
      }
    }
    return nullptr;
  }

  // 插入一个新节点
  V& InsertNode(const K& key) {
    Node* new_node = new Node(key, V{}, nullptr);
    new_node->next = head_->next;
    head_->next = new_node;
    size_++;
    return new_node->value;
  }

  V& InsertNode(K&& key) {
    Node* new_node = new Node(std::move(key), V{}, nullptr);
    new_node->next = head_->next;
    head_->next = new_node;
    size_++;
    return new_node->value;
  }

  friend class XSFSeparateChainingHashMap<K, V, Hash>;

  Node* head_{nullptr};
  Node* tail_{nullptr};
  size_t size_{0};
};

template <typename K, typename V, class Hash>
class XSFSeparateChainingHashMap {
 public:
  XSFSeparateChainingHashMap(size_t capacity = 4)
      : capacity_{CeilToPow2(capacity)},
        mask_{capacity_ - 1},
        table_{new Slot<K, V, Hash>[capacity_]} {}

  ~XSFSeparateChainingHashMap() { delete[] table_; }

  // 增、改
  V& operator[](const K& key) {
    // 扩容，负载因子：0.75
    if (size_ >= capacity_ * 0.75) {
      Resize(capacity_ * 2);
    }
    size_t index = HashIndex(key);
    if (table_[index].Contains(key)) {
      return table_[index][key];
    } else {
      size_++;
      return table_[index][key];
    }
  }

  V& operator[](K&& key) {
    // 扩容，负载因子：0.75
    if (size_ >= capacity_ * 0.75) {
      Resize(capacity_ * 2);
    }
    size_t index = HashIndex(std::move(key));
    if (table_[index].Contains(key)) {
      return table_[index][std::move(key)];
    } else {
      size_++;
      return table_[index][std::move(key)];
    }
  }

  bool Contains(const K& key) {
    size_t index = HashIndex(key);
    return table_[index].Contains(key);
  }

  bool Contains(K&& key) {
    size_t index = HashIndex(std::move(key));
    return table_[index].Contains(std::move(key));
  }

  size_t Count(const K& key) {
    size_t index = HashIndex(key);
    return table_[index].Count(key);
  }

  size_t Count(K&& key) {
    size_t index = HashIndex(std::move(key));
    return table_[index].Count(std::move(key));
  }

  // 删
  size_t Erase(const K& key) {
    size_t index = HashIndex(key);
    size_t count = table_[index].Erase(key);
    size_ -= count;
    return count;
  }

  size_t Erase(K&& key) {
    size_t index = HashIndex(std::move(key));
    size_t count = table_[index].Erase(std::move(key));
    size_ -= count;
    return count;
  }

  void Clear() {
    for (size_t i = 0; i < capacity_; i++) {
      table_[i].Clear();
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
    Slot<K, V, Hash>* new_table = new Slot<K, V, Hash>[new_capacity];
    // 将旧 table_ 中的元素重新哈希到新 table_ 中
    for (size_t i = 0; i < capacity_; i++) {
      Slot<K, V, Hash>& slot = table_[i];
      for (auto p = slot.head_->next; p != slot.tail_; p = p->next) {
        size_t index = hash_(p->key) & (new_capacity - 1);
        new_table[index][p->key] = std::move(p->value);
      }
    }
    delete[] table_;
    table_ = new_table;
    capacity_ = new_capacity;
    mask_ = capacity_ - 1;
  }

  // 将键映射到 table 的索引
  size_t HashIndex(const K& key) { return hash_(key) & mask_; }

  size_t HashIndex(K&& key) { return hash_(std::move(key)) & mask_; }

  Hash hash_{};

  size_t size_{0};  // 哈希表中存入的键值对个数
  size_t capacity_{4};
  size_t mask_{capacity_ - 1};

  Slot<K, V, Hash>* table_{nullptr};
};

}  // namespace xsf_data_structures

#endif  // XSF_SEPARATE_CHAINING_HASH_MAP_H