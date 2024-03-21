#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <list>
#include <unordered_map>

namespace xsf_data_structures {

class LRUCache {
 public:
  LRUCache(int capacity) { cap_ = capacity; }

  int get(int key) {
    if (key2node_.contains(key)) {
      // 找到 key
      makeRecently(key);
      return key2node_[key]->val;
    } else {
      // 未找到 key
      return -1;
    }
  }

  void put(int key, int value) {
    if (key2node_.contains(key)) {
      // 找到 key，更新
      key2node_[key]->val = value;
      makeRecently(key);
    } else {
      // 未找到 key，加入
      if (list_.size() + 1 > cap_) {
        // 加入后超出容量，需逐出
        popLeastRecently();
      }
      list_.emplace_back(key, value);
      key2node_[key] = --list_.end();
    }
  }

 private:
  void makeRecently(int key) {
    // 移至链表后端
    list_.push_back(*(key2node_[key]));
    list_.erase(key2node_[key]);
    key2node_[key] = --list_.end();
  }

  void popLeastRecently() {
    // 记录将逐出的 key
    int key = list_.front().key;
    // 逐出链表最前端元素
    list_.pop_front();
    // 删除对应映射
    key2node_.erase(key);
  }

  struct Node {
    int key;
    int val;
  };
  int cap_;
  std::list<Node> list_;
  std::unordered_map<int, std::list<Node>::iterator> key2node_;
};

}  // namespace xsf_data_structures

#endif  // LRU_CACHE_H