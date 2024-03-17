#ifndef XSF_TRIE_SET_H
#define XSF_TRIE_SET_H

#include "xsf_trie_map.h"

namespace xsf_data_structures {

class XSFTrieSet {
 public:
  // 增
  bool Insert(const std::string& key) {
    if (map_.Contains(key)) {
      return false;
    } else {
      map_[key] = kValue_;
      return true;
    }
  }

  bool Insert(std::string&& key) {
    if (map_.Contains(std::forward<std::string>(key))) {
      return false;
    } else {
      map_[std::forward<std::string>(key)] = kValue_;
      return true;
    }
  }

  // 删
  void Erase(const std::string& key) { map_.Erase(key); }

  void Erase(std::string&& key) { map_.Erase(std::forward<std::string>(key)); }

  void Clear() { map_.Clear(); }

  // 查
  bool Contains(const std::string& key) { return map_.Contains(key); }

  bool Contains(std::string&& key) {
    return map_.Contains(std::forward<std::string>(key));
  }

  // 在集合中寻找 query 的最短前缀
  std::string FindShortestPrefix(const std::string& query) {
    return map_.FindShortestPrefix(query);
  }

  std::string FindShortestPrefix(std::string&& query) {
    return map_.FindShortestPrefix(std::forward<std::string>(query));
  }

  // 在集合中寻找 query 的最长前缀
  std::string FindLongestPrefix(const std::string& query) {
    return map_.FindLongestPrefix(query);
  }

  std::string FindLongestPrefix(std::string&& query) {
    return map_.FindLongestPrefix(std::forward<std::string>(query));
  }

  // 在集合中搜索前缀为 prefix 的所有元素
  std::list<std::string> FindAllWithPrefix(const std::string& prefix) {
    return map_.FindKeysWithPrefix(prefix);
  }

  std::list<std::string> FindAllWithPrefix(std::string&& prefix) {
    return map_.FindKeysWithPrefix(std::forward<std::string>(prefix));
  }

  // 判断集合中是否存在前缀为 prefix 的元素
  bool ContainsPrefix(const std::string& prefix) {
    return map_.ContainsKeysWithPrefix(prefix);
  }

  bool ContainsPrefix(std::string&& prefix) {
    return map_.ContainsKeysWithPrefix(std::forward<std::string>(prefix));
  }

  // 搜索集合中符合 pattern 的所有元素（通配符 . 匹配任意字符）
  std::list<std::string> FindAllWithPattern(const std::string& pattern) {
    return map_.FindKeysWithPattern(pattern);
  }

  std::list<std::string> FindAllWithPattern(std::string&& pattern) {
    return map_.FindKeysWithPattern(std::forward<std::string>(pattern));
  }

  // 判断是否存在符合 pattern 的元素（通配符 . 匹配任意字符）
  bool ContainsPattern(const std::string& pattern) {
    return map_.ContainsKeysWithPattern(pattern);
  }

  bool ContainsPattern(std::string&& pattern) {
    return map_.ContainsKeysWithPattern(std::forward<std::string>(pattern));
  }

  // 工具函数
  size_t Size() const { return map_.Size(); }

  bool Empty() const { return map_.Empty(); }

 private:
  XSFTrieMap<char> map_;
  const char kValue_{'0'};
};

}  // namespace xsf_data_structures

#endif  // XSF_TRIE_SET_H