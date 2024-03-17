#ifndef XSF_LINKED_HASH_MAP_H
#define XSF_LINKED_HASH_MAP_H

#include <list>
#include <unordered_map>
#include <vector>

namespace xsf_data_structures {

// 新特性：可以顺序性访问所有 key，返回顺序即插入顺序
template <typename K, typename V, class Hash>
class XSFLinkedHashMap {
 private:
  struct Node {
    K key{};
    V value{};

    Node(const K& k, const V& v) : key(k), value(v) {}
    Node(const K& k, V&& v) : key(k), value(std::move(v)) {}
    Node(K&& k, const V& v) : key(std::move(k)), value(v) {}
    Node(K&& k, V&& v) : key(std::move(k)), value(std::move(v)) {}
  };

 public:
  // 顺序遍历所有 key，返回顺序即插入顺序
  std::vector<K> Keys() const {
    std::vector<K> keys(list_.size());
    for (const auto& node : list_) {
      keys.push_back(node.key);
    }
    return keys;
  }

  // 增、改
  V& operator[](const K& key) {
    auto it = map_.find(key);
    if (it != map_.end()) {
      return it->second->value;
    } else {
      list_.emplace_back(key, V{});
      map_[key] = --list_.end();
      return map_[key]->value;
    }
  }

  V& operator[](K&& key) {
    auto it = map_.find(key);
    if (it != map_.end()) {
      return it->second->value;
    } else {
      list_.emplace_back(std::forward<K>(key), V{});
      map_[std::forward<K>(key)] = --list_.end();
      return map_[key]->value;
    }
  }

  bool Contains(const K& key) const { return map_.find(key) != map_.end(); }

  bool Contains(K&& key) const { return map_.find(key) != map_.end(); }

  // 删
  bool Erase(const K& key) {
    auto it = map_.find(key);
    if (it == map_.end()) {
      // 若 key 本不存在，直接返回
      return false;
    } else {
      // 若 key 存在，删除 key 对应的节点
      list_.erase(it->second);
      map_.erase(it);
      return true;
    }
  }

  bool Erase(K&& key) {
    auto it = map_.find(key);
    if (it == map_.end()) {
      // 若 key 本不存在，直接返回
      return false;
    } else {
      // 若 key 存在，删除 key 对应的节点
      list_.erase(it->second);
      map_.erase(it);
      return true;
    }
  }

  void Clear() {
    list_.clear();
    map_.clear();
  }

  // 工具函数
  size_t Size() { return list_.size(); }

  bool Empty() { return list_.empty(); }

 private:
  std::list<Node> list_;
  std::unordered_map<K, typename std::list<Node>::iterator, Hash> map_;
};

}  // namespace xsf_data_structures

#endif  // XSF_LINKED_HASH_MAP_H