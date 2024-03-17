#ifndef XSF_LINKED_QUEUE_H
#define XSF_LINKED_QUEUE_H

#include "xsf_linked_list.h"

namespace xsf_data_structures {

template <typename T>
class XSFLinkedQueue {
 public:
  void Push(const T& data) { list.PushBack(data); }

  void Push(T&& data) { list.PushBack(std::move(data)); }

  template <typename... Args>
  void Emplace(Args&&... args) {
    list.EmplaceBack(std::forward<Args>(args)...);
  }

  void Pop() { list.PopFront(); }

  T& Front() { return list.Front(); }

  const T& Front() const { return list.Front(); }

  T& Back() { return list.Back(); }

  const T& Back() const { return list.Back(); }

  bool Empty() const { return list.Empty(); }

 private:
  XSFLinkedList<T> list;
};

}  // namespace xsf_data_structures

#endif  // XSF_LINKED_QUEUE_H