#ifndef XSF_TRIE_MAP_H
#define XSF_TRIE_MAP_H

#include <list>
#include <string>
#include <utility>

namespace xsf_data_structures {

template <typename V>
class XSFTrieMap {
 private:
  static const unsigned short kASCIICodeCount_{256};
  // TrieNode 节点本身只存储 value 字段，并没有一个字段来存储字符
  // 字符是通过节点在父节点的 children 数组中的索引确定的
  struct Node {
    V* value{nullptr};
    // children['a'] 代表指向字符 'a' 的子节点的指针
    Node* children[kASCIICodeCount_];

    Node() {
      for (size_t i = 0; i < kASCIICodeCount_; ++i) {
        children[i] = nullptr;
      }
    }
  };

 public:
  XSFTrieMap() = default;

  ~XSFTrieMap() { Clear(); }

  // 增、改
  V& operator[](const std::string& key) {
    auto [node, value] = InsertNodes(root_, key, 0);
    root_ = node;
    return value;
  }

  V& operator[](std::string&& key) {
    auto [node, value] = InsertNodes(root_, std::forward<std::string>(key), 0);
    root_ = node;
    return value;
  }

  // 删
  void Erase(const std::string& key) {
    if (Contains(key) == false) {
      return;
    }
    root_ = Erase(root_, key, 0);
  }

  void Erase(std::string&& key) {
    if (Contains(std::forward<std::string>(key)) == false) {
      return;
    }
    root_ = Erase(root_, std::forward<std::string>(key), 0);
  }

  void Clear() {
    Clear(root_);
    root_ = nullptr;
    size_ = 0;
  }

  // 查
  bool Contains(const std::string& key) const {
    Node* node = FindNode(root_, key, 0);
    return node != nullptr && node->value != nullptr;
  }

  bool Contains(std::string&& key) const {
    Node* node = FindNode(root_, std::forward<std::string>(key), 0);
    return node != nullptr && node->value != nullptr;
  }

  // 在所有键中寻找 query 的最短前缀
  std::string FindShortestPrefix(const std::string& query) const {
    Node* node = root_;
    // 从节点 node 开始搜索 key
    for (size_t i = 0; i < query.size(); i++) {
      if (node == nullptr) {
        // 无法向下搜索
        return "";
      }
      if (node->value != nullptr) {
        // 如果当前节点存储了 value，说明已经找到了最短前缀
        return query.substr(0, i);
      }
      // 继续向下搜索
      char c = query[i];
      node = node->children[c];
    }
    if (node != nullptr && node->value != nullptr) {
      // 如果 query 本身就是一个键，返回 query
      return query;
    }
    return "";
  }

  std::string FindShortestPrefix(std::string&& query) const {
    Node* node = root_;
    // 从节点 node 开始搜索 key
    for (size_t i = 0; i < query.size(); i++) {
      if (node == nullptr) {
        // 无法向下搜索
        return "";
      }
      if (node->value != nullptr) {
        // 如果当前节点存储了 value，说明已经找到了最短前缀
        return query.substr(0, i);
      }
      // 继续向下搜索
      char c = query[i];
      node = node->children[c];
    }
    if (node != nullptr && node->value != nullptr) {
      // 如果 query 本身就是一个键，返回 query
      return query;
    }
    return "";
  }

  // 在所有键中寻找 query 的最长前缀
  std::string FindLongestPrefix(const std::string& query) const {
    Node* node = root_;
    // 记录前缀的最大长度
    size_t max_length = 0;
    // 从节点 node 开始搜索 key
    for (size_t i = 0; i < query.size(); ++i) {
      if (node == nullptr) {
        // 无法向下搜索
        break;
      }
      if (node->value != nullptr) {
        // 找到一个键是 query 的前缀，更新前缀的最大长度
        max_length = i;
      }
      // 继续向下搜索
      char c = query[i];
      node = node->children[c];
    }
    if (node != nullptr && node->value != nullptr) {
      // 如果 query 本身就是一个键，返回 query
      return query;
    }
    return query.substr(0, max_length);
  }

  std::string FindLongestPrefix(std::string&& query) const {
    Node* node = root_;
    // 记录前缀的最大长度
    size_t max_length = 0;
    // 从节点 node 开始搜索 key
    for (size_t i = 0; i < query.size(); ++i) {
      if (node == nullptr) {
        // 无法向下搜索
        break;
      }
      if (node->value != nullptr) {
        // 找到一个键是 query 的前缀，更新前缀的最大长度
        max_length = i;
      }
      // 继续向下搜索
      char c = query[i];
      node = node->children[c];
    }
    if (node != nullptr && node->value != nullptr) {
      // 如果 query 本身就是一个键，返回 query
      return query;
    }
    return query.substr(0, max_length);
  }

  // 搜索前缀为 prefix 的所有键
  std::list<std::string> FindKeysWithPrefix(const std::string& prefix) const {
    std::list<std::string> keys;
    // 找到在 Trie 树中匹配 prefix 的那个节点
    Node* node = FindNode(root_, prefix, 0);
    if (node == nullptr) {
      return keys;
    }
    // DFS 遍历以 node 为根的这棵 Trie 树
    std::string path = prefix;
    Traverse(node, path, keys);
    return keys;
  }

  std::list<std::string> FindKeysWithPrefix(std::string&& prefix) const {
    std::list<std::string> keys;
    // 找到在 Trie 树中匹配 prefix 的那个节点
    Node* node = FindNode(root_, std::forward<std::string>(prefix), 0);
    if (node == nullptr) {
      return keys;
    }
    // DFS 遍历以 node 为根的这棵 Trie 树
    std::string path = prefix;
    Traverse(node, path, keys);
    return keys;
  }

  // 判断是否存在前缀为 prefix 的键
  bool ContainsKeysWithPrefix(const std::string& prefix) const {
    Node* node = FindNode(root_, prefix, 0);
    return node != nullptr;
  }

  bool ContainsKeysWithPrefix(std::string&& prefix) const {
    Node* node = FindNode(root_, std::forward<std::string>(prefix), 0);
    return node != nullptr;
  }

  // 搜索符合 pattern 的所有键（通配符 . 匹配任意字符）
  std::list<std::string> FindKeysWithPattern(const std::string& pattern) const {
    std::list<std::string> keys;
    std::string path;
    Traverse(root_, path, pattern, 0, keys);
    return keys;
  }

  std::list<std::string> FindKeysWithPattern(std::string&& pattern) const {
    std::list<std::string> keys;
    std::string path;
    Traverse(root_, path, std::forward<std::string>(pattern), 0, keys);
    return keys;
  }

  // 判断是否存在符合 pattern 的键（通配符 . 匹配任意字符）
  bool ContainsKeysWithPattern(const std::string& pattern) const {
    return Match(root_, pattern, 0);
  }

  bool ContainsKeysWithPattern(std::string&& pattern) const {
    return Match(root_, std::forward<std::string>(pattern), 0);
  }

  // 工具函数
  size_t Size() const { return size_; }

  bool Empty() const { return size_ == 0; }

 private:
  // 向以 node 为根的 Trie 树中插入 key[i..]
  // 返回插入后的根节点、新节点的 value 的引用
  std::pair<Node*, V&> InsertNodes(Node* node, const std::string& key,
                                   size_t i) {
    if (node == nullptr) {
      // 如果树枝不存在，新建一个节点
      node = new Node();
    }
    if (i == key.size()) {
      // 如果已经到达字符串末尾，说明已经插入了整个字符串
      if (node->value == nullptr) {
        // 如果当前节点没有 value，说明这是一个新的字符串
        size_++;
        node->value = new V{};
      }
      return {node, *(node->value)};
    }
    char c = key[i];
    // 在 node 的 children 数组中“插入字符 c”
    // 递归地从 node->children[c] 开始插入 key[i+1..]
    auto [child, value] = InsertNodes(node->children[c], key, i + 1);
    node->children[c] = child;
    return {node, value};
  }

  std::pair<Node*, V&> InsertNodes(Node* node, std::string&& key, size_t i) {
    if (node == nullptr) {
      // 如果树枝不存在，新建一个节点
      node = new Node();
    }
    if (i == key.size()) {
      // 如果已经到达字符串末尾，说明已经插入了整个字符串
      if (node->value == nullptr) {
        // 如果当前节点没有 value，说明这是一个新的字符串
        size_++;
        node->value = new V{};
      }
      return {node, *(node->value)};
    }
    char c = key[i];
    // 在 node 的 children 数组中“插入字符 c”
    // 递归地从 node->children[c] 开始插入 key[i+1..]
    auto [child, value] =
        InsertNodes(node->children[c], std::forward<std::string>(key), i + 1);
    node->children[c] = child;
    return {node, value};
  }

  // 在以 node 为根的 Trie 树中删除 key[i..]（已保证 key 存在）
  // 返回删除后的根节点
  Node* Erase(Node* node, const std::string& key, size_t i) {
    if (i == key.size()) {
      // 如果已经到达字符串末尾，说明已经删除了整个字符串
      size_--;
      delete node->value;
      node->value = nullptr;
    } else {
      char c = key[i];
      // 递归地从 node->children[c] 开始删除 key[i+1..]
      node->children[c] = Erase(node->children[c], key, i + 1);
    }

    // 后序位置，递归路径上的节点可能需要被清理
    if (node->value != nullptr) {
      // 如果该 TireNode 存储着 val，不需要被清理
      return node;
    }
    // 检查该 TrieNode 是否还有后缀
    for (size_t i = 0; i < kASCIICodeCount_; ++i) {
      if (node->children[i] != nullptr) {
        // 只要存在一个子节点（后缀树枝），就不需要被清理
        return node;
      }
    }
    // 既没有存储 val，也没有后缀树枝，则该节点需要被清理
    delete node;
    return nullptr;
  }

  Node* Erase(Node* node, std::string&& key, size_t i) {
    if (i == key.size()) {
      // 如果已经到达字符串末尾，说明已经删除了整个字符串
      size_--;
      delete node->value;
      node->value = nullptr;
    } else {
      char c = key[i];
      // 递归地从 node->children[c] 开始删除 key[i+1..]
      node->children[c] =
          Erase(node->children[c], std::forward<std::string>(key), i + 1);
    }

    // 后序位置，递归路径上的节点可能需要被清理
    if (node->value != nullptr) {
      // 如果该 TireNode 存储着 val，不需要被清理
      return node;
    }
    // 检查该 TrieNode 是否还有后缀
    for (size_t i = 0; i < kASCIICodeCount_; ++i) {
      if (node->children[i] != nullptr) {
        // 只要存在一个子节点（后缀树枝），就不需要被清理
        return node;
      }
    }
    // 既没有存储 val，也没有后缀树枝，则该节点需要被清理
    delete node;
    return nullptr;
  }

  // 删除以 node 为根的 Trie 树
  void Clear(Node* node) {
    if (node == nullptr) {
      return;
    }
    for (size_t i = 0; i < kASCIICodeCount_; ++i) {
      if (node->children[i] != nullptr) {
        Clear(node->children[i]);
      }
    }
    delete node;
  }

  // 从节点 node 开始搜索 key，如果存在返回对应节点，否则返回 null
  Node* FindNode(Node* node, const std::string& key, size_t i) const {
    if (node == nullptr) {
      return nullptr;
    }
    if (i == key.size()) {
      return node;
    }
    char c = key[i];
    return FindNode(node->children[c], key, i + 1);
  }

  Node* FindNode(Node* node, std::string&& key, size_t i) const {
    if (node == nullptr) {
      return nullptr;
    }
    if (i == key.size()) {
      return node;
    }
    char c = key[i];
    return FindNode(node->children[c], std::forward<std::string>(key), i + 1);
  }

  // 遍历以 node 节点为根的 Trie 树，找到所有键
  void Traverse(Node* node, std::string& path,
                std::list<std::string>& keys) const {
    if (node == nullptr) {
      // 到达 Trie 树底部叶子结点
      return;
    }
    if (node->value != nullptr) {
      // 找到一个 key，添加到结果列表中
      keys.push_back(path);
    }

    // 回溯算法遍历框架
    for (size_t i = 0; i < kASCIICodeCount_; ++i) {
      if (node->children[i] != nullptr) {
        // 做选择：仅当 children[i] 不为空时才添加字符到 path 末尾
        path.push_back(i);
        // 递归地遍历 children[i]
        Traverse(node->children[i], path, keys);
        // 撤销选择：回溯时删除 path 末尾的字符
        path.pop_back();
      }
    }
  }

  // 遍历函数，尝试在「以 node 为根的 Trie 树中」匹配 pattern[i..]
  void Traverse(Node* node, std::string& path, const std::string& pattern,
                size_t i, std::list<std::string>& keys) const {
    if (node == nullptr) {
      // 树枝不存在，即匹配失败
      return;
    }
    if (i == pattern.size()) {
      // pattern 匹配完成
      if (node->value != nullptr) {
        // 如果这个节点存储着 val，则找到一个匹配的键
        keys.emplace_back(path);
      }
      return;
    }
    char c = pattern[i];
    if (c == '.') {
      // pattern[i] 是通配符，可以变化成任意字符
      // 多叉树（回溯算法）遍历框架
      for (size_t j = 0; j < kASCIICodeCount_; ++j) {
        if (node->children[j] != nullptr) {
          // 做选择
          path.push_back(j);
          // 递归地遍历 children[j]
          Traverse(node->children[j], path, pattern, i + 1, keys);
          // 撤销选择
          path.pop_back();
        }
      }
    } else {
      // pattern[i] 是普通字符 c
      if (node->children[c] != nullptr) {
        // 做选择
        path.push_back(c);
        // 递归地遍历 children[c]
        Traverse(node->children[c], path, pattern, i + 1, keys);
        // 撤销选择
        path.pop_back();
      }
    }
  }

  void Traverse(Node* node, std::string& path, std::string&& pattern, size_t i,
                std::list<std::string>& keys) const {
    if (node == nullptr) {
      // 树枝不存在，即匹配失败
      return;
    }
    if (i == pattern.size()) {
      // pattern 匹配完成
      if (node->value != nullptr) {
        // 如果这个节点存储着 val，则找到一个匹配的键
        keys.emplace_back(path);
      }
      return;
    }
    char c = pattern[i];
    if (c == '.') {
      // pattern[i] 是通配符，可以变化成任意字符
      // 多叉树（回溯算法）遍历框架
      for (size_t j = 0; j < kASCIICodeCount_; ++j) {
        if (node->children[j] != nullptr) {
          // 做选择
          path.push_back(j);
          // 递归地遍历 children[j]
          Traverse(node->children[j], path, std::forward<std::string>(pattern),
                   i + 1, keys);
          // 撤销选择
          path.pop_back();
        }
      }
    } else {
      // pattern[i] 是普通字符 c
      if (node->children[c] != nullptr) {
        // 做选择
        path.push_back(c);
        // 递归地遍历 children[c]
        Traverse(node->children[c], path, std::forward<std::string>(pattern),
                 i + 1, keys);
        // 撤销选择
        path.pop_back();
      }
    }
  }

  // 从 node 节点开始匹配 pattern[i..]，返回是否成功匹配
  bool Match(Node* node, const std::string& pattern, size_t i) const {
    if (node == nullptr) {
      // 树枝不存在，即匹配失败
      return false;
    }
    if (i == pattern.size()) {
      // 模式串走到头了，看看匹配到的是否是一个键
      return node->value != nullptr;
    }
    char c = pattern[i];
    if (c != '.') {
      // 没有遇到通配符
      // 从 node.children[c] 节点开始匹配 pattern[i+1..]
      return Match(node->children[c], pattern, i + 1);
    } else {
      // 遇到通配符
      // pattern[i] 可以变化成任意字符，尝试所有可能，只要遇到一个匹配成功就返回
      for (size_t j = 0; j < kASCIICodeCount_; ++j) {
        if (Match(node->children[j], pattern, i + 1)) {
          return true;
        }
      }
      // 都没有匹配
      return false;
    }
  }

  bool Match(Node* node, std::string&& pattern, size_t i) const {
    if (node == nullptr) {
      // 树枝不存在，即匹配失败
      return false;
    }
    if (i == pattern.size()) {
      // 模式串走到头了，看看匹配到的是否是一个键
      return node->value != nullptr;
    }
    char c = pattern[i];
    if (c != '.') {
      // 没有遇到通配符
      // 从 node.children[c] 节点开始匹配 pattern[i+1..]
      return Match(node->children[c], std::forward<std::string>(pattern),
                   i + 1);
    } else {
      // 遇到通配符
      // pattern[i] 可以变化成任意字符，尝试所有可能，只要遇到一个匹配成功就返回
      for (size_t j = 0; j < kASCIICodeCount_; ++j) {
        if (Match(node->children[j], std::forward<std::string>(pattern),
                  i + 1)) {
          return true;
        }
      }
      // 都没有匹配
      return false;
    }
  }

  size_t size_{0};
  Node* root_{nullptr};
};

}  // namespace xsf_data_structures

#endif  // XSF_TRIE_MAP_H