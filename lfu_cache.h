#ifndef LFU_CACHE_H
#define LFU_CACHE_H

#include <list>
#include <unordered_map>

namespace xsf_data_structures {

class LFUCache {
 public:
  LFUCache(int capacity) { cap_ = capacity; }

  int get(int key) {
    if (key2node_.contains(key)) {
      // 找到 key
      increaseFreq(key);
      return key2node_[key]->val;
    } else {
      // 未找到 key
      return -1;
    }
  }

  void put(int key, int value) {
    if (key2node_.contains(key)) {
      // key 已存在，更新
      increaseFreq(key);
      key2node_[key]->val = value;
    } else {
      // key 不存在，加入
      if (key2node_.size() + 1 > cap_) {
        // 加入后超出容量，需逐出
        removeMinFreqKey();
      }
      // 新加入，即对应频率、最小频率为 1
      key2freq_[key] = 1;
      min_freq_ = 1;
      // 加入到对应频率的节点链表中
      freq2nodes_[1].emplace_back(key, value);
      // 记录 key 到节点的映射
      key2node_[key] = --freq2nodes_[1].end();
    }
  }

 private:
  void increaseFreq(int key) {
    // 找到 key 对应的频率
    int freq = key2freq_[key];
    // 更新 key 到频率的映射
    key2freq_[key]++;

    // 在新频率对应的节点链表中插入新节点
    auto& nodes_list_cur = freq2nodes_[freq + 1];
    nodes_list_cur.emplace_back(key, key2node_[key]->val);
    // 删除原频率对应的节点链表中的节点
    auto& nodes_list_pre = freq2nodes_[freq];
    nodes_list_pre.erase(key2node_[key]);
    if (nodes_list_pre.empty()) {
      // 若原节点链表为空
      // 删除对应频率到链表的映射
      freq2nodes_.erase(freq);
      if (freq == min_freq_) {
        // 更新最小频率
        min_freq_++;
      }
    }

    // 更新 key 到节点的映射
    key2node_[key] = --nodes_list_cur.end();
  }

  void removeMinFreqKey() {
    // 找到最小频率对应的节点链表
    auto& nodes_list = freq2nodes_[min_freq_];
    // 找到要逐出的（访问频率最少且最旧）的节点对应的 key
    int key = nodes_list.front().key;
    // 从链表中逐出节点
    nodes_list.pop_front();
    if (nodes_list.empty()) {
      // 若链表变为空，则移除最小频率到链表的映射
      freq2nodes_.erase(min_freq_);
    }
    // 移除对应 key 到节点的映射
    key2node_.erase(key);
    // 移除对应 key 到频率的映射
    key2freq_.erase(key);
  }

  struct Node {
    int key;
    int val;
  };
  int cap_;
  int min_freq_;
  std::unordered_map<int, int> key2freq_;
  std::unordered_map<int, std::list<Node>> freq2nodes_;
  std::unordered_map<int, std::list<Node>::iterator> key2node_;
};

}  // namespace xsf_data_structures

#endif  // LFU_CACHE_H