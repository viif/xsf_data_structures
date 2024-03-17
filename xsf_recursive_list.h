#ifndef XSF_RECURSIVE_LIST_H
#define XSF_RECURSIVE_LIST_H

namespace xsf_data_structures {

// 递归实现单链表
template <typename T>
class XSFRecursiveList {
 private:
  // 单链表节点
  struct Node {
    T data{};
    Node* next{nullptr};

    Node() = default;
    Node(const T& d, Node* n) : data(d), next(n) {}
    Node(T&& d, Node* n) : data(std::move(d)), next(n) {}
  };

 public:
  ~XSFRecursiveList() { Clear(); }

  // 增
  void PushFront(const T& data) {
    auto new_node{new Node(data, head_)};
    head_ = new_node;
    size_++;
  }

  void PushFront(T&& data) {
    auto new_node{new Node(std::move(data), head_)};
    head_ = new_node;
    size_++;
  }

  template <typename... Args>
  T& EmplaceFront(Args&&... args) {
    auto new_node{new Node(T(std::forward<Args>(args)...), head_)};
    head_ = new_node;
    size_++;
    return head_->data;
  }

  void PushBack(const T& data) {
    head_ = PushBack(head_, data);
    size_++;
  }

  void PushBack(T&& data) {
    head_ = PushBack(head_, std::move(data));
    size_++;
  }

  template <typename... Args>
  T& EmplaceBack(Args&&... args) {
    head_ = EmplaceBack(head_, std::forward<Args>(args)...);
    size_++;
    return GetLastNode(head_)->data;
  }

  void Insert(size_t index, const T& data) {
    CheckPosition(index);
    head_ = Insert(head_, index, data);
    size_++;
  }

  void Insert(size_t index, T&& data) {
    CheckPosition(index);
    head_ = Insert(head_, index, std::move(data));
    size_++;
  }

  template <typename... Args>
  T& Emplace(size_t index, Args&&... args) {
    CheckPosition(index);
    head_ = Emplace(head_, index, std::forward<Args>(args)...);
    size_++;
    return GetNode(index)->data;
  }

  // 删
  void PopFront() {
    if (head_ == nullptr) {
      return;
    }
    auto temp{head_};
    head_ = head_->next;
    delete temp;
    size_--;
  }

  void PopBack() {
    if (head_ == nullptr) {
      return;
    }
    head_ = PopBack(head_);
    size_--;
  }

  void Erase(size_t index) {
    CheckElement(index);
    head_ = Erase(head_, index);
    size_--;
  }

  // 查、改
  T& Front() {
    if (head_ == nullptr) {
      throw std::out_of_range("XSFRecursiveList::Front()");
    }
    return head_->data;
  }

  const T& Front() const {
    if (head_ == nullptr) {
      throw std::out_of_range("XSFRecursiveList::Front()");
    }
    return head_->data;
  }

  T& Back() {
    if (head_ == nullptr) {
      throw std::out_of_range("XSFRecursiveList::Back()");
    }
    return GetLastNode(head_)->data;
  }

  const T& Back() const {
    if (head_ == nullptr) {
      throw std::out_of_range("XSFRecursiveList::Back()");
    }
    return GetLastNode(head_)->data;
  }

  T& At(size_t index) {
    CheckElement(index);
    return GetNode(index)->data;
  }

  const T& At(size_t index) const {
    CheckElement(index);
    return GetNode(index)->data;
  }

  // 工具函数
  size_t Size() const { return size_; }

  bool Empty() const { return size_ == 0; }

  void Clear() {
    // 递归删除所有节点
    head_ = Clear(head_);
    size_ = 0;
  }

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

  // 返回 index 对应的 Node
  // 注意：请保证传入的 index 是合法的
  Node* GetNode(size_t index) { return GetNode(head_, index); }

  // 返回「从 node 开始的第 index 个链表节点」
  Node* GetNode(Node* node, size_t index) {
    // base case
    if (index == 0) {
      return node;
    }
    // 返回 「从 node.next 开始的第 index - 1 个链表节点」
    return GetNode(node->next, index - 1);
  }

  Node* GetLastNode(Node* node) {
    if (node->next == nullptr) {
      return node;
    }
    return GetLastNode(node->next);
  }

  // x -> y -> z -> nullptr
  // x -> y -> nullptr
  Node* PopBack(Node* node) {
    if (node->next == nullptr) {
      // node 就是最后一个节点 z，让自己直接消失
      delete node;
      return nullptr;
    }
    // y -> nullptr
    node->next = PopBack(node->next);
    return node;
  }

  // x -> y -> z -> nullptr
  // x -> z -> nullptr
  Node* Erase(Node* node, size_t index) {
    if (index == 0) {
      // node 就是要删除的节点 y，让自己消失并返回 z
      auto temp{node->next};
      delete node;
      return temp;
    }
    // x -> z
    node->next = Erase(node->next, index - 1);
    return node;
  }

  // x -> y -> nullptr
  // x -> y -> z -> nullptr
  Node* PushBack(Node* node, const T& data) {
    if (node == nullptr) {
      // node 是最后一个节点的下一位置，直接创建一个新节点 z 并返回
      return new Node(data, nullptr);
    }
    // y -> z
    node->next = PushBack(node->next, data);
    return node;
  }

  // x -> y -> nullptr
  // x -> y -> z -> nullptr
  Node* PushBack(Node* node, T&& data) {
    if (node == nullptr) {
      // node 是最后一个节点的下一位置，直接创建一个新节点 z 并返回
      return new Node(std::move(data), nullptr);
    }
    // y -> z
    node->next = PushBack(node->next, std::move(data));
    return node;
  }

  // x -> y -> nullptr
  // x -> y -> z -> nullptr
  template <typename... Args>
  Node* EmplaceBack(Node* node, Args&&... args) {
    if (node == nullptr) {
      // node 是最后一个节点的下一位置，直接创建一个新节点 z 并返回
      return new Node(T(std::forward<Args>(args)...), nullptr);
    }
    // y -> z
    node->next = EmplaceBack(node->next, std::forward<Args>(args)...);
    return node;
  }

  // x -> z -> nullptr
  // x -> y -> z -> nullptr
  Node* Insert(Node* node, size_t index, const T& data) {
    if (index == 0) {
      // node 是要插入的位置，创建一个新节点 y 并返回
      return new Node(data, node);
    }
    // x -> y
    node->next = Insert(node->next, index - 1, data);
    return node;
  }

  // x -> z -> nullptr
  // x -> y -> z -> nullptr
  Node* Insert(Node* node, size_t index, T&& data) {
    if (index == 0) {
      // node 是要插入的位置，创建一个新节点 y 并返回
      return new Node(std::move(data), node);
    }
    // x -> y
    node->next = Insert(node->next, index - 1, std::move(data));
    return node;
  }

  // x -> z -> nullptr
  // x -> y -> z -> nullptr
  template <typename... Args>
  Node* Emplace(Node* node, size_t index, Args&&... args) {
    if (index == 0) {
      // node 是要插入的位置，创建一个新节点 y 并返回
      return new Node(T(std::forward<Args>(args)...), node);
    }
    // x -> y
    node->next = Emplace(node->next, index - 1, std::forward<Args>(args)...);
    return node;
  }

  Node* Clear(Node* node) {
    if (node == nullptr) {
      return nullptr;
    }
    auto temp{node->next};
    // 删除自己
    delete node;
    // 递归删除下一个节点
    return Clear(temp);
  }

  size_t size_{0};
  Node* head_{nullptr};
};

}  // namespace xsf_data_structures

#endif  // XSF_RECURSIVE_LIST_H