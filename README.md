# 备战秋招

---
## C++
### 目标
1. 语言本身：读完Effective C++、Effective modern C++、侯捷视频复习（有精力将Modern C++ Tutorial补了）
2. 并发编程：并发编程实战第二版、（c/c++的协程）
3. 网络编程：高性能服务器编程、Linux多线程服务端编程

---

## 数据结构算法

### 目标

**考虑的书籍：**

- 《剑指OFFER》：百分之七十的公司，手撕算法涉及到上面的题目，所以建立大家至少刷两遍，培养写代码的感觉。
- 《labuladong的算法小抄》：总结各种类型题目的套路解法。
- 《程序员代码面试指南》：开阔视野

**刷题方式：**

对于每一道题目，都想做出最优解，不满足一种解法，分析多种[算法](https://www.nowcoder.com/jump/super-jump/word?word=算法)的时间空间复杂度。这里要注意，解不出来的题目也不要太花时间在上面，如何去平衡呢，大概1小时都没思路，就可以看看提示并尝试求解。



分类刷：数组、二分、二叉树、动态规划



**刷题网站：**

- acwing
- leetcode

---

## 操作系统

### 目标

- 《Linux内核设计与实现》
  - 这本书重点关注「第 3 章进程管理」、「第 5 章系统调用」、「第12章内存管理」、「第13章虚拟文件系统」、「第 15 章进程地址空间」

- 部分 Linux 内核原理，如内存管理、文件系统、虚拟内存等

---

## 计算机网络

### 目标

- Linux内核协议栈的实现

---

## 数据库

### 目标

- sql的使用：《MySQL必知必会》

- sql的索引和并发控制：

  - 《高性能MySQL》

  - 《MySQL技术内幕》

    > 这两本主要对索引、innodb存储引擎、锁、并发控制讲得比较清楚，挑对应章节看。

---

## 设计模式





---

## 技能/工具

- Git：在项目练习时贯串使用，要熟悉VS code和命令行的操作
- CMake/Makefile

- others:   https://github.com/szza/LearningNote/tree/master/7.Tools

---

## 项目

项目部分我直接好家伙，尤其是使用 C++ 岗位的同学，几乎人手一个 Server，确实没有其他的什么项目，为了让自己的项目能有不一样的地方，大家可以根据自己的项目，采用一些性能工具，对多个库进行测试，可视化出来结论，这样不就算一个小的创新了。

有项目自然是好，不多对于应届生而言，基础部分更加的看中了，只是有项目会聊的更自如。

你的项目一定需要融入到自己的想法，因为大家都知道使用 Libevent，Muduo库，如果大家都一样，就没法表现出自己的优点了。可以在上层增加一些业务逻辑或者融入微服务的中间件，这里面就会涉及到分布式的一些算法如raft，自然就突出自己的特点了。





- WebServer：实现基本框架后加入其他功能：如代理
- RPC
- protobuf
- 不通过服务器中转消息实现 P2P 聊天，类似 QQ，这里会涉及到 UDP 打洞、NAT 转换等知识
- 手动实现协程

- 『C++』高性能分布式服务器框架 的项目：https://github.com/sylar-yin/sylar   B站视频教程：https://www.bilibili.com/video/av53602631?from=search&seid=9029288577396826503

