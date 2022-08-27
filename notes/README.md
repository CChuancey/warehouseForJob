# 剑指Offer

## 面试需要的基础知识

---

```c++
// operator= 中的copy-swap
class Test{
  	Test();
    Test(const Test& oth);
    Test& operator=(const Test& oth){
		if(&oth==this) {
            return;
        }
        Test tmp(oth);
        std::swap(oth,tmp);
    }
};

// 单例模式

```

