# 数据结构算法
---

## 刷题笔记

- 边界和特殊情况的考虑：空集合
- 第一个和最后一个元素的考虑

### 13 找重复的数

**1. ** 给定一个长度为 n 的整数数组 `nums`，数组中所有的数字都在 0∼n−1的范围内。

数组中某些数字是重复的，但不知道有几个数字重复了，也不知道每个数字重复了几次。

请找出数组中任意一个重复的数字。

**注意**：如果某些数字不在 0∼n−1 的范围内，或数组中不包含重复数字，则返回 -1

```c++
// 在O(n)的时间内，找出重复的数
// 关键思想：置换法
class Solution {
public:
    int duplicateInArray(vector<int>& nums) {
        for(int it : nums){
            if(it<0||it>=nums.size()){
                return -1;
            }
        }
        // 占坑法（置位操作）
        for(int i=0;i<nums.size();i++){
            while(i!=nums[i]){
                if(nums[i]==nums[nums[i]]){
                    return nums[i];
                }
                swap(nums[i],nums[nums[i]]);
            }
            
        }

        return -1;
    }
};
```

**2. ** 不修改数组找出重复的数字

给定一个长度为 n+1 的数组`nums`，数组中所有的数均在 1∼n的范围内，其中 n≥1。

请找出数组中任意一个重复的数，但不能修改输入的数组。

#### 数据范围

1≤n≤1000

#### 样例

```bash
给定 nums = [2, 3, 5, 4, 3, 2, 6, 7]。

返回 2 或 3。
```

注意：只能使用 O(1) 的额外空间。

```c++
// 传入的数组是不能修改的，还需要O(1)的额外空间，不能定义数组等其他数据结构
// 1. 抽屉原理判定，1-n的区间，n+1的数==》必有重复的数
// 2. 二分查找区间：不是有序的数组，但是数所在的区间是固定的，可以二分区间，查找数在哪个区间内发生了重复。
// 3. 判定数是否在某个区间内重复的原理：依然是抽屉原理，在[l,m]区间内，数组中所有的在此区间的数如果等于区间长度，那么此区间没有重复；否则，如果在此区间的数的个数如果比区间长度长，说明在此区间发生了重复。
class Solution {
public:
    int duplicateInArray(vector<int>& nums) {
        // 二分法查找
        auto l = 1;
        auto r = nums.size()-1;
        while(l<r){
            auto m = (l+r)/2;
            auto sum = 0;
            for(auto it : nums) {
                sum += (it >=l && it <= m);
            }
            // 如果数组中在[l,m]区间内的数比区间长度多，说明在[l,m]中的某个数重复出现了
            if(sum > m - l + 1){
                r = m;
            }else{
                l = m + 1;
            }
        }
        return l;
    }
};
```

### 15 二维数组的查找

在一个二维数组中，每一行都按照从左到右递增的顺序排序，每一列都按照从上到下递增的顺序排序。

请完成一个函数，输入这样的一个二维数组和一个整数，判断数组中是否含有该整数。

#### 数据范围

二维数组中元素个数范围 【[0,1000][0,1000]】

#### 样例

```bash
输入数组：

[
  [1,2,8,9]，
  [2,4,9,12]，
  [4,7,10,13]，
  [6,8,11,15]
]

如果输入查找数值为7，则返回true，

如果输入查找数值为5，则返回false。
```

```c++
// 可先考虑特殊情况，将false的情况先列出：vector或者vector<vector>为空
// 观察特性，右上角元素大于等于左边元素&&小于等于下边的元素
// 从右上角开始查找，即可实现O(n+m)的时间复杂度查找
class Solution {
public:
    bool searchArray(vector<vector<int>> array, int target) {
        if(array.empty()||array[0].empty()){
            return false;
        }
        
        int i=0;
        int j=array[0].size()-1;
        while(i<array.size()&&j>=0){
            if(array[i][j]<target){
                i++;
            }else if(array[i][j]>target){
                j--;
            }else{
                return true;
            }
        }
        
        return false;
    }
};

```

### 19. 二叉树的下一个节点

给定一棵二叉树的其中一个节点，请找出中序遍历序列的下一个节点。

**注意：**

- 如果给定的节点是中序遍历序列的最后一个，则返回空节点;
- 二叉树一定不为空，且给定的节点一定不是空节点；

#### 数据范围

树中节点数量 [][0,100][0,100]。

#### 样例

```bash
假定二叉树是：[2, 1, 3, null, null, null, null]， 给出的是值等于2的节点。

则应返回值等于3的节点。

解释：该二叉树的结构如下，2的后继节点是3。
  2
 / \
1   3
```

```C++

// 对于二叉树，其特殊结点考虑根节点和叶子结点

// 中序遍历的二叉树查找后继
// 1. 其结点有右子树，则其后继结点在其右子树的最左边
// 2. 其结点没有右子树，其后继结点应往上找，找到作为父节点的左子结点的祖先，此时该祖先的父节点为后继结点

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode *father;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL), father(NULL) {}
 * };
 */
class Solution {
public:
    TreeNode* inorderSuccessor(TreeNode* p) {
        if(p->right){
            p=p->right;
            while(p->left){
                p=p->left;
            }
            return p;
        }
        
        while(p->father){
            if(p==p->father->left){
                return p->father;
            }
            p = p->father;
        }
        
        return nullptr;
    }
};

```

### 29. 删除链表中重复的节点

​	在一个排序的链表中，存在重复的节点，请删除该链表中重复的节点，重复的节点不保留。

#### 数据范围

链表中节点 val 值取值范围 [0,100]。
链表长度 [0,100]。

#### 样例1

```bash
输入：1->2->3->3->4->4->5

输出：1->2->5
```

#### 样例2

```bash
输入：1->1->1->2->3

输出：2->3
```

```c++
// 此题没给出头结点，可以虚拟出一个头结点☆
// 关键技巧：在头结点上构造出一个新的链表出来：新增一个遍历指针：只有当原链表的某个元素不重复才更新遍历指针的下一个结点（p->next）&&只有当扫描下一个结点没有发生重复时才更新编译指针（p）
// 这样在构造新链表时就可以忽略重复的元素

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode* deleteDuplication(ListNode* head) {
        ListNode* pre = new ListNode(-1);
        pre->next = head;
        
        
        ListNode* dummy = pre;
        while(dummy->next){
            int v = dummy->next->val;
            ListNode* p = dummy->next;
            while(p->next&&p->next->val==v){
                p=p->next;
            }
            // std::cout << "P:" << p->val << std::endl;
            
            if(dummy->next->next==p->next){
                dummy=dummy->next;
            }else{
                dummy->next=p->next;
            }
            // std::cout << dummy->val << std::endl;
        }
        
        return pre->next;
    }
};
```

### 30 二分模板

1. `r = mid`的情况

```C++
while(l<r){
    int mid = (l + r) >> 1;
    if(check) r = mid;
    else l = mid + 1; 
}
```

2. `l = mid`的情况

```C++
while(l<r){
    int mid = (l + r + 1) >> 1;
    if(check) l = mid;
    else r = mid - 1;
}
```