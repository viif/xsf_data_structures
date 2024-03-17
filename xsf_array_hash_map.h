#ifndef XSF_ARRAY_HASH_MAP_H
#define XSF_ARRAY_HASH_MAP_H

#include <random>
#include <unordered_map>
#include <vector>

namespace xsf_data_structures {

// 新特性：可以在 O(1) 时间内等概率地随机返回一个 key
template <typename K, typename V, class Hash>
class XSFArrayHashMap {
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
  // 随机返回一个 key
  K Pop() {
    std::uniform_int_distribution<size_t> dist(0, vec_.size() - 1);
    size_t index = dist(gen);
    return vec_[index].key;
  }

  // 增、改
  V& operator[](const K& key) {
    auto it = map_.find(key);
    if (it != map_.end()) {
      return vec_[it->second].value;
    } else {
      vec_.emplace_back(key, V{});
      map_[key] = vec_.size() - 1;
      return vec_.back().value;
    }
  }

  V& operator[](K&& key) {
    auto it = map_.find(std::forward<K>(key));
    if (it != map_.end()) {
      return vec_[it->second].value;
    } else {
      vec_.emplace_back(std::forward<K>(key), V{});
      map_[std::forward<K>(key)] = vec_.size() - 1;
      return vec_.back().value;
    }
  }

  bool Contains(const K& key) const { return map_.find(key) != map_.end(); }

  bool Contains(K&& key) const {
    return map_.find(std::forward<K>(key)) != map_.end();
  }

  // 删
  bool Erase(const K& key) {
    if (map_.find(key) == map_.end()) {
      return false;
    }
    size_t index = map_[key];
    map_.erase(key);
    if (index != vec_.size() - 1) {
      // 交换 vec_ 中要删除的节点和最后一个节点
      std::swap(vec_[index], vec_.back());
      // 更新 map_ 中原先 vec_ 中最后一个节点的索引
      map_[vec_[index].key] = index;
    }
    vec_.pop_back();
    return true;
  }

  bool Erase(K&& key) {
    if (map_.find(std::forward<K>(key)) == map_.end()) {
      return false;
    }
    size_t index = map_[std::forward<K>(key)];
    map_.erase(std::forward<K>(key));
    if (index != vec_.size() - 1) {
      // 交换 vec_ 中要删除的节点和最后一个节点
      std::swap(vec_[index], vec_.back());
      // 更新 map_ 中原先 vec_ 中最后一个节点的索引
      map_[vec_[index].key] = index;
    }
    vec_.pop_back();
    return true;
  }

  void Clear() {
    vec_.clear();
    map_.clear();
  }

  // 工具函数
  size_t Size() const { return vec_.size(); }

  bool Empty() const { return vec_.empty(); }

 private:
  std::random_device rd{};
  std::mt19937 gen{rd()};

  std::vector<Node> vec_;
  std::unordered_map<K, size_t, Hash> map_;
};

}  // namespace xsf_data_structures

#endif  // XSF_ARRAY_HASH_MAP_H