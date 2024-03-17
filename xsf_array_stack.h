#ifndef XSF_ARRAY_STACK_H
#define XSF_ARRAY_STACK_H

#include "xsf_array_list.h"

namespace xsf_data_structures {

template <typename T>
class XSFArrayStack {
 public:
  XSFArrayStack(size_t capacity = 2) : list(capacity) {}

  void Push(const T& data) { list.PushBack(data); }

  void Push(T&& data) { list.PushBack(std::move(data)); }

  template <typename... Args>
  void Emplace(Args&&... args) {
    list.EmplaceBack(std::forward<Args>(args)...);
  }

  void Pop() { list.PopBack(); }

  T& Top() { return list.Back(); }

  const T& Top() const { return list.Back(); }

  bool Empty() const { return list.Empty(); }

 private:
  XSFArrayList<T> list;
};

}  // namespace xsf_data_structures

#endif  // XSF_ARRAY_STACK_H