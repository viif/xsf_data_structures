#ifndef XSF_LINKED_LIST_H
#define XSF_LINKED_LIST_H

namespace xsf_data_structures {

template <typename T>
class XSFLinkedList {
 private:
  // 双链表节点
  struct Node {
    T data{};
    Node* prev{nullptr};
    Node* next{nullptr};

    Node(const T& d, Node* p, Node* n) : data(d), prev(p), next(n) {}
    Node(T&& d, Node* p, Node* n) : data(std::move(d)), prev(p), next(n) {}
  };

 public:
  // 迭代器
  class const_iterator {
   public:
    const_iterator() = default;

    const T& operator*() const { return Retrieve(); }

    const_iterator& operator++() {
      current = current->next;
      return *this;
    }

    const_iterator operator++(int) {
      const_iterator old{*this};
      ++(*this);
      return old;
    }

    const_iterator& operator--() {
      current = current->prev;
      return *this;
    }

    const_iterator operator--(int) {
      const_iterator old{*this};
      --(*this);
      return old;
    }

    bool operator==(const const_iterator& rhs) const {
      return current == rhs.current;
    }

    bool operator!=(const const_iterator& rhs) const { return !(*this == rhs); }

   protected:
    Node* current{nullptr};

    // Protected helper in const_iterator that returns the object
    // stored at the current position. Can be called by all
    // three versions of operator* without any type conversions.
    T& Retrieve() const { return current->data; }

    const_iterator(Node* p) : current{p} {}

    friend class XSFLinkedList<T>;
  };

  class iterator : public const_iterator {
   public:
    iterator() = default;

    T& operator*() { return const_iterator::Retrieve(); }

    const T& operator*() const { return const_iterator::operator*(); }

    iterator& operator++() {
      this->current = this->current->next;
      return *this;
    }

    iterator operator++(int) {
      iterator old{*this};
      ++(*this);
      return old;
    }

    iterator& operator--() {
      this->current = this->current->prev;
      return *this;
    }

    iterator operator--(int) {
      iterator old{*this};
      --(*this);
      return old;
    }

   protected:
    iterator(Node* p) : const_iterator{p} {}

    friend class XSFLinkedList<T>;
  };

 public:
  XSFLinkedList() { Init(); }

  XSFLinkedList(const XSFLinkedList& rhs) {
    Init();
    for (auto& x : rhs) {
      PushBack(x);
    }
  }

  XSFLinkedList& operator=(const XSFLinkedList& rhs) {
    XSFLinkedList copy{rhs};
    std::swap(*this, copy);
    return *this;
  }

  XSFLinkedList(XSFLinkedList&& rhs)
      : size_(rhs.size_), head_(rhs.head_), tail_(rhs.tail_) {
    rhs.size_ = 0;
    rhs.head_ = nullptr;
    rhs.tail_ = nullptr;
  }

  XSFLinkedList& operator=(XSFLinkedList&& rhs) {
    if (this != &rhs) {
      std::swap(size_, rhs.size_);
      std::swap(head_, rhs.head_);
      std::swap(tail_, rhs.tail_);
    }
    return *this;
  }

  ~XSFLinkedList() {
    Clear();
    delete head_;
    delete tail_;
  }

  // 增
  void PushFront(const T& value) {
    head_->next = head_->next->prev = new Node(value, head_, head_->next);
    size_++;
  }

  void PushFront(T&& value) {
    head_->next = head_->next->prev =
        new Node(std::move(value), head_, head_->next);
    size_++;
  }

  template <typename... Args>
  T& EmplaceFront(Args&&... args) {
    head_->next = head_->next->prev =
        new Node(T(std::forward<Args>(args)...), head_, head_->next);
    size_++;
    return head_->next->data;
  }

  void PushBack(const T& value) {
    tail_->prev = tail_->prev->next = new Node(value, tail_->prev, tail_);
    size_++;
  }

  void PushBack(T&& value) {
    tail_->prev = tail_->prev->next =
        new Node(std::move(value), tail_->prev, tail_);
    size_++;
  }

  template <typename... Args>
  T& EmplaceBack(Args&&... args) {
    tail_->prev = tail_->prev->next =
        new Node(T(std::forward<Args>(args)...), tail_->prev, tail_);
    size_++;
    return tail_->prev->data;
  }

  void Insert(size_t index, const T& value) {
    Node* p = GetNode(index);
    p->prev = p->prev->next = new Node(value, p->prev, p);
    size_++;
  }

  void Insert(size_t index, T&& value) {
    Node* p = GetNode(index);
    p->prev = p->prev->next = new Node(std::move(value), p->prev, p);
    size_++;
  }

  template <typename... Args>
  void Emplace(size_t index, Args&&... args) {
    Node* p = GetNode(index);
    p->prev = p->prev->next =
        new Node(T(std::forward<Args>(args)...), p->prev, p);
    size_++;
  }

  // Insert x before pos.
  iterator Insert(iterator pos, const T& value) {
    Node* p = pos.current;
    size_++;
    return iterator(p->prev = p->prev->next = new Node(value, p->prev, p));
  }

  // Insert x before pos.
  iterator Insert(iterator pos, T&& value) {
    Node* p = pos.current;
    size_++;
    return iterator(p->prev = p->prev->next =
                        new Node(std::move(value), p->prev, p));
  }

  // 删
  void PopFront() {
    Node* p = head_->next;
    head_->next = p->next;
    p->next->prev = head_;
    delete p;
    size_--;
  }

  void PopBack() {
    Node* p = tail_->prev;
    tail_->prev = p->prev;
    p->prev->next = tail_;
    delete p;
    size_--;
  }

  // Erase item at pos.
  iterator Erase(iterator pos) {
    Node* p = pos.current;
    iterator ret{p->next};
    p->prev->next = p->next;
    p->next->prev = p->prev;
    delete p;
    size_--;
    return ret;
  }

  void Erase(size_t index) {
    Node* p = GetNode(index);
    p->prev->next = p->next;
    p->next->prev = p->prev;
    delete p;
    size_--;
  }

  // 查、改
  iterator begin() { return iterator(head_->next); }

  const_iterator begin() const { return const_iterator(head_->next); }

  iterator end() { return iterator(tail_); }

  const_iterator end() const { return const_iterator(tail_); }

  T& Front() { return *begin(); }

  const T& Front() const { return *begin(); }

  T& Back() { return *--end(); }

  const T& Back() const { return *--end(); }

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
    while (!Empty()) PopFront();
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

  Node* GetNode(size_t index) {
    CheckElement(index);
    if (index < (size_ >> 1)) {
      Node* p = head_->next;
      for (size_t i = 0; i < index; i++) {
        p = p->next;
      }
      return p;
    } else {
      Node* p = tail_;
      for (size_t i = size_; i > index; i--) {
        p = p->prev;
      }
      return p;
    }
    return nullptr;
  }

  void Init() {
    head_ = new Node(T{}, nullptr, nullptr);
    tail_ = new Node(T{}, head_, nullptr);
    head_->next = tail_;
    size_ = 0;
  }

  size_t size_{0};
  Node* head_{nullptr};
  Node* tail_{nullptr};
};

}  // namespace xsf_data_structures

#endif  // XSF_LINKED_LIST_H