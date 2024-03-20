# xsf_data_structures

使用 C++ 实现了一些常用的数据结构。

| 类名                       | 说明                                                         |
| -------------------------- | ------------------------------------------------------------ |
| XSFArray                   | 定长数组                                                     |
| XSFArrayList               | 变长数组                                                     |
| XSFLinkedList              | 双向链表                                                     |
| XSFArrayStack              | 栈，基于变长数组                                             |
| XSFLinkedStack             | 栈，基于双向链表                                             |
| XSFArrayDeque              | 双端队列，基于环形数组                                       |
| XSFArrayQueue              | 队列，基于双端队列                                           |
| XSFLinkedQueue             | 队列，基于双向链表                                           |
| XSFRingBuffer              | 环形缓冲区                                                   |
| XSFSeparateChainingHashMap | 哈希映射，使用拉链法解决冲突                                 |
| XSFLinearProbingHashMap    | 哈希映射，使用线性探查法解决冲突                             |
| XSFHashSet                 | 哈希集合，使用线性探查法解决冲突                             |
| XSFLinkedHashMap           | 映射，基于哈希链表，特性：可以顺序性访问所有  key，返回顺序即插入顺序 |
| XSFLinkedHashSet           | 集合，基于哈希链表，特性：可以顺序性访问所有  key，返回顺序即插入顺序 |
| XSFArrayHashMap            | 映射，基于哈希数组，特性：可以在 O(1)  时间内等概率地随机返回一个 key |
| XSFArrayHashSet            | 集合，基于哈希数组，特性：可以在 O(1)  时间内等概率地随机返回一个 key |
| XSFRecursiveList           | 单向链表，各种操作以递归实现                                 |
| XSFTreeMap                 | 映射，基于普通 BST                                           |
| XSFTrieMap                 | 映射，基于前缀树                                             |
| XSFTrieSet                 | 集合，基于前缀树                                             |
