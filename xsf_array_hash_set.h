#ifndef XSF_ARRAY_HASH_SET_H
#define XSF_ARRAY_HASH_SET_H

#include "xsf_array_hash_map.h"

namespace xsf_data_structures {

// 新特性：可以在 O(1) 时间内等概率地随机返回一个 key
template <typename K, class Hash>
class XSFArrayHashSet {
 public:
  // 随机返回一个 key
  K Pop() { return map_.Pop(); }

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
    if (map_.Contains(std::forward<K>(key))) {
      return false;
    } else {
      map_[std::forward<K>(key)] = kValue_;
      return true;
    }
  }

  // 删
  bool Erase(const K& key) { return map_.Erase(key); }

  bool Erase(K&& key) { return map_.Erase(std::forward<K>(key)); }

  void Clear() { map_.Clear(); }

  // 查
  bool Contains(const K& key) const { return map_.Contains(key); }

  bool Contains(K&& key) const { return map_.Contains(std::forward<K>(key)); }

  // 工具函数
  size_t Size() const { return map_.Size(); }

  bool Empty() const { return map_.Empty(); }

 private:
  XSFArrayHashMap<K, char, Hash> map_;
  const char kValue_{'0'};
};

}  // namespace xsf_data_structures

#endif  // XSF_ARRAY_HASH_SET_H