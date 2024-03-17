#ifndef XSF_LINKED_HASH_SET_H
#define XSF_LINKED_HASH_SET_H

#include "xsf_linked_hash_map.h"

namespace xsf_data_structures {

// 新特性：可以顺序性访问所有 key，返回顺序即插入顺序
template <typename K, class Hash>
class XSFLinkedHashSet {
 public:
  // 顺序遍历所有 key，返回顺序即插入顺序
  std::vector<K> Keys() const { return map_.Keys(); }

  // 增
  bool Insert(const K& key) {
    if (map_.Contains(key)) {
      return false;
    } else {
      map_[key] = kValue_;
      return true;
    }
  }

  bool Insert(K&& key) {
    if (map_.Contains(std::move(key))) {
      return false;
    } else {
      map_[std::move(key)] = kValue_;
      return true;
    }
  }

  // 删
  bool Erase(const K& key) { return map_.Erase(key); }

  bool Erase(K&& key) { return map_.Erase(std::move(key)); }

  void Clear() { map_.Clear(); }

  // 查
  bool Contains(const K& key) const { return map_.Contains(key); }

  bool Contains(K&& key) const { return map_.Contains(std::move(key)); }

  // 工具函数
  size_t Size() const { return map_.Size(); }

  bool Empty() const { return map_.Empty(); }

 private:
  XSFLinkedHashMap<K, char, Hash> map_;
  const char kValue_{'0'};
};

}  // namespace xsf_data_structures

#endif  // XSF_LINKED_HASH_SET_H