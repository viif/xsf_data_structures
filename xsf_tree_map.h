#ifndef XSF_TREE_MAP_H
#define XSF_TREE_MAP_H

#include <list>
#include <utility>
#include <vector>

namespace xsf_data_structures {

// 以普通 BST 为底层实现的 map
template <typename K, typename V, class Compare>
class XSFTreeMap {
 private:
  struct Node {
    K key{};
    V value{};
    Node* left{nullptr};
    Node* right{nullptr};
    // 记录以该节点为根的 BST 有多少个节点
    size_t size_{1};

    Node() = default;

    Node(const K& k, const V& v, Node* l, Node* r, size_t s)
        : key(k), value(v), left(l), right(r), size_(s) {}

    Node(const K& k, V&& v, Node* l, Node* r, size_t s)
        : key(k), value(std::move(v)), left(l), right(r), size_(s) {}

    Node(K&& k, const V& v, Node* l, Node* r, size_t s)
        : key(std::move(k)), value(v), left(l), right(r), size_(s) {}

    Node(K&& k, V&& v, Node* l, Node* r, size_t s)
        : key(std::move(k)), value(std::move(v)), left(l), right(r), size_(s) {}

    Node(const Node&) = default;
  };

 public:
  XSFTreeMap() = default;

  ~XSFTreeMap() { Clear(); }

  // 增、改
  V& operator[](const K& key) {
    auto [new_root, new_value] = InsertNode(root_, key);
    root_ = new_root;
    return new_value;
  }

  V& operator[](K&& key) {
    auto [new_root, new_value] = InsertNode(root_, std::forward<K>(key));
    root_ = new_root;
    return new_value;
  }

  // 删
  void EraseMax() { root_ = EraseMax(root_); }

  void EraseMin() { root_ = EraseMin(root_); }

  void Erase(const K& key) { root_ = Erase(root_, key); }

  void Erase(K&& key) { root_ = Erase(root_, std::forward<K>(key)); }

  void Clear() {
    Clear(root_);
    root_ = nullptr;
  }

  // 查
  bool Contains(const K& key) { return FindNode(root_, key) != nullptr; }

  bool Contains(K&& key) {
    return FindNode(root_, std::forward<K>(key)) != nullptr;
  }

  // 查找小于等于 key 的最大的键，如果不存在则返回 false
  bool Floor(const K& key, K& result) {
    Node* node = Floor(root_, key);
    if (node == nullptr) {
      return false;
    }
    result = node->key;
    return true;
  }

  bool Floor(K&& key, K& result) {
    Node* node = Floor(root_, std::forward<K>(key));
    if (node == nullptr) {
      return false;
    }
    result = node->key;
    return true;
  }

  // 查找大于等于 key 的最小的键
  bool Ceiling(const K& key, K& result) {
    Node* node = Ceiling(root_, key);
    if (node == nullptr) {
      return false;
    }
    result = node->key;
    return true;
  }

  bool Ceiling(K&& key, K& result) {
    Node* node = Ceiling(root_, std::forward<K>(key));
    if (node == nullptr) {
      return false;
    }
    result = node->key;
    return true;
  }

  // 返回小于 key 的键的个数
  size_t Rank(const K& key) const { return Rank(root_, key); }

  size_t Rank(K&& key) const { return Rank(root_, std::forward<K>(key)); }

  // 返回索引为 i 的键，i 从 0 开始计算
  K Select(size_t i) const {
    if (i >= Size()) {
      throw std::out_of_range("Index out of range");
    }
  }

  // 工具函数
  size_t Size() const { return Size(root_); }

  bool Empty() const { return root_ == nullptr; }

  // 从小到大返回所有键
  std::vector<K> Keys() const {
    std::vector<K> keys(Size());
    InOrderTraverse(root_, keys);
    return keys;
  }

  // 从小到大返回闭区间 [min, max] 中的键（仅提供左值引用版本）
  std::list<K> Keys(const K& min, const K& max) const {
    std::list<K> keys;
    Keys(root_, keys, min, max);
    return keys;
  }

 private:
  // 在以 node 为根的 BST 中插入一个新节点
  // 返回插入后的根节点、新节点的 value 的引用
  std::pair<Node*, V&> InsertNode(Node* node, const K& key) {
    if (node == nullptr) {
      Node* new_node = new Node(key, V{}, nullptr, nullptr, 1);
      return {new_node, new_node->value};
    }
    if (compare_(key, node->key)) {
      // key < node->key
      auto [new_left, new_value] = InsertNode(node->left, key);
      node->left = new_left;
      node->size_ = Size(node->left) + 1 + Size(node->right);
      return {node, new_value};
    } else if (compare_(node->key, key)) {
      // key > node->key
      auto [new_right, new_value] = InsertNode(node->right, key);
      node->right = new_right;
      node->size_ = Size(node->left) + 1 + Size(node->right);
      return {node, new_value};
    } else {
      // key == node->key
      return {node, node->value};
    }
  }

  std::pair<Node*, V&> InsertNode(Node* node, K&& key) {
    if (node == nullptr) {
      Node* new_node = new Node(std::move(key), V{}, nullptr, nullptr, 1);
      return {new_node, new_node->value};
    }
    if (compare_(key, node->key)) {
      // key < node->key
      auto [new_left, new_value] = InsertNode(node->left, std::forward<K>(key));
      node->left = new_left;
      node->size_ = Size(node->left) + 1 + Size(node->right);
      return {node, new_value};
    } else if (compare_(node->key, key)) {
      // key > node->key
      auto [new_right, new_value] =
          InsertNode(node->right, std::forward<K>(key));
      node->right = new_right;
      node->size_ = Size(node->left) + 1 + Size(node->right);
      return {node, new_value};
    } else {
      // key == node->key
      return {node, node->value};
    }
  }

  // 删除以 node 为根的 BST 中的最大节点
  Node* EraseMax(Node* node) {
    if (node->right == nullptr) {
      // node 就是最大节点
      Node* left = node->left;
      delete node;
      return left;
    }
    node->right = EraseMax(node->right);
    node->size_--;
    return node;
  }

  // 删除以 node 为根的 BST 中的最小节点
  Node* EraseMin(Node* node) {
    if (node->left == nullptr) {
      // node 就是最小节点
      Node* right = node->right;
      delete node;
      return right;
    }
    node->left = EraseMin(node->left);
    node->size_--;
    return node;
  }

  // 删除以 node 为根的 BST 中 key 对应的节点
  Node* Erase(Node* node, const K& key) {
    if (node == nullptr) {
      // key 对应的节点不存在
      return nullptr;
    }
    if (compare_(key, node->key)) {
      // key < node->key，找左子树
      node->left = Erase(node->left, key);
    } else if (compare_(node->key, key)) {
      // key > node->key，找右子树
      node->right = Erase(node->right, key);
    } else {
      // key == node->key，找到了要删除的节点 node
      if (node->left == nullptr) {
        // node 无左子树
        Node* right = node->right;
        delete node;
        return right;
      }
      if (node->right == nullptr) {
        // node 无右子树
        Node* left = node->left;
        delete node;
        return left;
      }
      // node 有左右子树
      // 不直接交换节点中的数据，而是交换节点，实现解耦
      // 复制左子树的最大节点作为新的根节点
      Node* left_max{new Node(*FindMax(node->left))};
      // 删除左子树的最大节点，并用 left_max 替换 node
      left_max->left = EraseMax(node->left);
      left_max->right = node->right;
      delete node;
      node = left_max;
    }
    node->size_ = Size(node->left) + 1 + Size(node->right);
    return node;
  }

  Node* Erase(Node* node, K&& key) {
    if (node == nullptr) {
      // key 对应的节点不存在
      return nullptr;
    }
    if (compare_(key, node->key)) {
      // key < node->key，找左子树
      node->left = Erase(node->left, std::forward<K>(key));
    } else if (compare_(node->key, key)) {
      // key > node->key，找右子树
      node->right = Erase(node->right, std::forward<K>(key));
    } else {
      // key == node->key，找到了要删除的节点 node
      if (node->left == nullptr) {
        // node 无左子树
        Node* right = node->right;
        delete node;
        return right;
      }
      if (node->right == nullptr) {
        // node 无右子树
        Node* left = node->left;
        delete node;
        return left;
      }
      // node 有左右子树
      // 不直接交换节点中的数据，而是交换节点，实现解耦
      // 复制左子树的最大节点作为新的根节点
      Node* left_max{new Node(*FindMax(node->left))};
      // 删除左子树的最大节点，并用 left_max 替换 node
      left_max->left = EraseMax(node->left);
      left_max->right = node->right;
      delete node;
      node = left_max;
    }
    node->size_ = Size(node->left) + 1 + Size(node->right);
    return node;
  }

  // 删除以 node 为根的 BST
  void Clear(Node* node) {
    if (node == nullptr) {
      return;
    }
    Clear(node->left);
    Clear(node->right);
    delete node;
  }

  // 在以 node 为根的 BST 中查找最大节点
  Node* FindMax(Node* node) {
    while (node->right != nullptr) {
      node = node->right;
    }
    return node;
  }

  // 在以 node 为根的 BST 中查找 key 对应的节点
  Node* FindNode(Node* node, const K& key) {
    if (node == nullptr) {
      // key 对应的节点不存在
      return nullptr;
    }
    if (compare_(key, node->key)) {
      // key < node->key
      return FindNode(node->left, key);
    } else if (compare_(node->key, key)) {
      // key > node->key
      return FindNode(node->right, key);
    } else {
      // key == node->key
      return node;
    }
  }

  Node* FindNode(Node* node, K&& key) {
    if (node == nullptr) {
      // key 对应的节点不存在
      return nullptr;
    }
    if (compare_(key, node->key)) {
      // key < node->key
      return FindNode(node->left, std::forward<K>(key));
    } else if (compare_(node->key, key)) {
      // key > node->key
      return FindNode(node->right, std::forward<K>(key));
    } else {
      // key == node->key
      return node;
    }
  }

  // 在以 node 为根的 BST 中查找具有小于等于 key 的最大的键的节点
  Node* Floor(Node* node, const K& key) {
    if (node == nullptr) {
      return nullptr;
    }
    if (compare_(key, node->key)) {
      // key < node->key，去左子树找
      return Floor(node->left, key);
    } else if (compare_(node->key, key)) {
      // key > node->key，去右子树找
      Node* temp = Floor(node->right, key);
      if (temp == nullptr) {
        // 右子树中没有小于等于 key 的最大的键的节点
        // 那么 node 就是小于等于 key 的最大的键的节点
        return node;
      }
      return temp;
    } else {
      // key == node->key
      return node;
    }
  }

  Node* Floor(Node* node, K&& key) {
    if (node == nullptr) {
      return nullptr;
    }
    if (compare_(key, node->key)) {
      // key < node->key，去左子树找
      return Floor(node->left, std::forward<K>(key));
    } else if (compare_(node->key, key)) {
      // key > node->key，去右子树找
      Node* temp = Floor(node->right, std::forward<K>(key));
      if (temp == nullptr) {
        // 右子树中没有小于等于 key 的最大的键的节点
        // 那么 node 就是小于等于 key 的最大的键的节点
        return node;
      }
      return temp;
    } else {
      // key == node->key
      return node;
    }
  }

  // 在以 node 为根的 BST 中查找具有大于等于 key 的最小的键的节点
  Node* Ceiling(Node* node, const K& key) {
    if (node == nullptr) {
      return nullptr;
    }
    if (compare_(key, node->key)) {
      // key < node->key，去左子树找
      Node* temp = Ceiling(node->left, key);
      if (temp == nullptr) {
        // 左子树中没有大于等于 key 的最小的键的节点
        // 那么 node 就是大于等于 key 的最小的键的节点
        return node;
      }
      return temp;
    } else if (compare_(node->key, key)) {
      // key > node->key，去右子树找
      return Ceiling(node->right, key);
    } else {
      // key == node->key
      return node;
    }
  }

  Node* Ceiling(Node* node, K&& key) {
    if (node == nullptr) {
      return nullptr;
    }
    if (compare_(key, node->key)) {
      // key < node->key，去左子树找
      Node* temp = Ceiling(node->left, std::forward<K>(key));
      if (temp == nullptr) {
        // 左子树中没有大于等于 key 的最小的键的节点
        // 那么 node 就是大于等于 key 的最小的键的节点
        return node;
      }
      return temp;
    } else if (compare_(node->key, key)) {
      // key > node->key，去右子树找
      return Ceiling(node->right, std::forward<K>(key));
    } else {
      // key == node->key
      return node;
    }
  }

  // 返回以 node 为根的 BST 中小于 key 的键的个数
  size_t Rank(Node* node, const K& key) const {
    if (node == nullptr) {
      return 0;
    }
    if (compare_(key, node->key)) {
      // key < node->key，node 及其右子树中的键都大于 key，故只去左子树中找
      return Rank(node->left, key);
    } else if (compare_(node->key, key)) {
      // key > node->key，node 及其左子树中的键都小于 key，还要去右子树中找
      return Size(node->left) + 1 + Rank(node->right, key);
    } else {
      // key == node->key，只有左子树中的键小于 key
      return Size(node->left);
    }
  }

  size_t Rank(Node* node, K&& key) const {
    if (node == nullptr) {
      return 0;
    }
    if (compare_(key, node->key)) {
      // key < node->key，node 及其右子树中的键都大于 key，故只去左子树中找
      return Rank(node->left, std::forward<K>(key));
    } else if (compare_(node->key, key)) {
      // key > node->key，node 及其左子树中的键都小于 key，还要去右子树中找
      return Size(node->left) + 1 + Rank(node->right, std::forward<K>(key));
    } else {
      // key == node->key，只有左子树中的键小于 key
      return Size(node->left);
    }
  }

  // 返回以 node 为根的 BST 中索引为 i 的那个节点
  Node* Select(Node* node, size_t i) const {
    if (node == nullptr) {
      return nullptr;
    }
    // 左子树中的节点个数即是 node 的索引
    size_t left_size = Size(node->left);
    if (i < left_size) {
      // i 在左子树中
      return Select(node->left, i);
    } else if (i > left_size) {
      // i 在右子树中
      return Select(node->right, i - left_size - 1);
    } else {
      // i == left_size
      return node;
    }
  }

  // 中序遍历以 node 为根的 BST
  void InOrderTraverse(Node* node, std::vector<K>& keys) const {
    if (node == nullptr) {
      return;
    }
    InOrderTraverse(node->left, keys);
    keys.push_back(node->key);
    InOrderTraverse(node->right, keys);
  }

  // 中序遍历以 node 为根的 BST，将闭区间 [min, max] 中的键放入 keys
  void InOrderTraverse(Node* node, std::list<K>& keys, const K& min,
                       const K& max) const {
    if (node == nullptr) {
      return;
    }

    bool min_less_than_node = compare_(min, node->key);

    if (min_less_than_node) {
      // min < node->key，才有必要去左子树找
      InOrderTraverse(node->left, keys, min, max);
    }

    bool node_less_than_min = compare_(node->key, min);
    bool max_less_than_node = compare_(max, node->key);

    if (!node_less_than_min && !max_less_than_node) {
      // min <= node->key <= max
      keys.push_back(node->key);
    }

    bool node_less_than_max = compare_(node->key, max);

    if (node_less_than_max) {
      // max > node->key，才有必要去右子树找
      InOrderTraverse(node->right, keys, min, max);
    }
  }

  size_t Size(Node* node) const {
    if (node == nullptr) {
      return 0;
    }
    return node->size_;
  }

  Compare compare_{};
  Node* root_{nullptr};
};

}  // namespace xsf_data_structures

#endif  // XSF_TREE_MAP_H