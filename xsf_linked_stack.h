#ifndef XSF_LINKED_STACK_H
#define XSF_LINKED_STACK_H

#include "xsf_linked_list.h"

namespace xsf_data_structures {

template <typename T>
class XSFLinkedStack {
 public:
  void Push(const T& data) { list.PushFront(data); }

  void Push(T&& data) { list.PushFront(std::move(data)); }

  template <typename... Args>
  void Emplace(Args&&... args) {
    list.EmplaceFront(std::forward<Args>(args)...);
  }

  void Pop() { list.PopFront(); }

  T& Top() { return list.Front(); }

  const T& Top() const { return list.Front(); }

  bool Empty() const { return list.Empty(); }

 private:
  XSFLinkedList<T> list;
};

}  // namespace xsf_data_structures

#endif  // XSF_LINKED_STACK_H