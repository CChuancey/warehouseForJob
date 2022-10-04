#include <algorithm>
#include <climits>
#include <exception>
#include <iostream>
#include <limits>
#include <list>
#include <memory>
#include <mutex>
#include <stack>
#include <stdexcept>
#include <thread>
#include <type_traits>

// 避免恶性竞争的方式
// 0.互斥量
// 1.原子操作
// 2.事务的方式：所需的数据和读取都存储在事务日志中，让后将之前的操作进行合并，再进行提交。

namespace mutex_ns {
// 通过实例化std::mutex 创建互斥量实例

std::list<int> someList;
std::mutex somMutex;

void addToList(int v) {
    // lock_guard简化了mutex的使用，定义时lock，销毁时unlock
    std::lock_guard<std::mutex> guard(somMutex);
    someList.push_back(v);
}

bool listFind(int v) {
    std::lock_guard<std::mutex> guard(somMutex);
    return std::find(someList.begin(), someList.end(), v) != someList.end();
}

/**************************线程安全的栈 ** Begin*****************************/
template <typename T>
class ThreadSafeStack {
public:
    ThreadSafeStack();
    ThreadSafeStack(const ThreadSafeStack&);
    // 栈禁止赋值操作
    ThreadSafeStack& operator=(const ThreadSafeStack<T>&) = delete;

    void push(T);
    std::shared_ptr<T> pop();
    void pop(T&);
    bool empty() const;

private:
    std::stack<T> mData;
    mutable std::mutex mMutex;
};

template <typename T>
ThreadSafeStack<T>::ThreadSafeStack() : mData(std::stack<T>()) {}

template <typename T>
ThreadSafeStack<T>::ThreadSafeStack(const ThreadSafeStack<T>& oth) {
    // oth的锁
    std::lock_guard<std::mutex> lock(oth.mMutex);
    mData = oth.mData;
}

template <typename T>
void ThreadSafeStack<T>::push(T v) {
    std::lock_guard<std::mutex> lock(mMutex);
    mData.push(v);
}

template <typename T>
std::shared_ptr<T> ThreadSafeStack<T>::pop() {
    std::lock_guard<std::mutex> lock(mMutex);
    if (mData.empty()) {
        throw std::exception();
    }
    std::shared_ptr<T> res(std::make_shared<T>(mData.top()));
    mData.pop();
    return res;
}

template <typename T>
void ThreadSafeStack<T>::pop(T& v) {
    std::lock_guard<std::mutex> lock(mMutex);
    if (mData.empty()) {
        throw std::exception();
    }
    v = mData.top();
    mData.pop();
}

template <typename T>
bool ThreadSafeStack<T>::empty() const {
    std::lock_guard<std::mutex> lock(mMutex);
    return mData.empty();
}

/*********************************线程安全的栈* End************************/

// 死锁问题的解决,交换a,b的值，a,b可能被其他线程修改
class Sth {
    friend void safeSwap(Sth&, Sth&);
    friend void swap(Sth&, Sth&);

public:
    Sth(int v);

private:
    int mValue;
    std::mutex mMutex;
};

void swap(Sth& a, Sth& b) {
    // std::swap(a.mMutex, b.mMutex);
    std::swap(a.mValue, b.mValue);
}

void safeSwap(Sth& a, Sth& b) {
    if (&a == &b) {
        return;
    }

    /**
     * @brief
     * 写法1，通过std::lock模板函数同时对两个锁上锁；之后使用std::lock_guard管理锁
     *（std::adopt_lock参数的作用是使其采用现成的锁，创建
     *时不会对mutex再加一层锁）
     *
     */
    // {
    //     std::lock(a.mMutex, b.mValue);
    //     std::lock_guard<std::mutex> lockGuard1(a.mMutex, std::adopt_lock);
    //     std::lock_guard<std::mutex> lockGuard2(b.mMutex, std::adopt_lock);
    //     swap(a, b);
    // }

    /**
     * @brief C++17写法
     * C++ 17使用了自动模板参数推导，原始的类对象写法:
     * std::scoped_lock<std::mutex,std::mutex> lock(...)
     *
     */
    // {
    //     std::scoped_lock lock(a.mMutex, b.mMutex);
    //     swap(a, b);
    // }
}

/*************************************************************************************/

/**
 * @brief 避免死锁的方式
 * 1.避免嵌套锁
 * 2.一旦持锁，必须避免调用由用户提供的程序接口（其中包含上锁/获取锁的操作）
 * 3.以固定的顺序获取锁
 * 4.按层级加锁
 * 5.将准则推广到锁操作以外，任何同步机制导致的循环等待都有可能产生死锁
 */

// 手动模拟层级锁的实现
class HierarchialMutex {
public:
    explicit HierarchialMutex(unsigned long v);

    void lock();
    void unlock();
    bool tryLock();

private:
    void checkHierarchyValue() const;
    void updateHierarchValue();

    std::mutex mMutex;
    unsigned long const mHierarchialValue;
    unsigned long mPreviousValue{};
    static thread_local unsigned long CurrentThreadValue;
};
// 变量类型的最值头文件：limits.h
thread_local unsigned long HierarchialMutex::CurrentThreadValue = ULONG_MAX;

HierarchialMutex::HierarchialMutex(unsigned long v) : mHierarchialValue(v) {}

void HierarchialMutex::lock() {
    // check valid
    checkHierarchyValue();
    mMutex.lock();
    updateHierarchValue();
}

void HierarchialMutex::unlock() {
    if (mHierarchialValue != CurrentThreadValue) {
        throw std::logic_error("unlock different hierarchy");
    }
    // 解锁时更新记录上次现成的等级
    mPreviousValue = CurrentThreadValue;
    mMutex.unlock();
}

bool HierarchialMutex::tryLock() {
    checkHierarchyValue();
    if (!mMutex.try_lock()) {
        return false;
    }
    updateHierarchValue();
    return true;
}

void HierarchialMutex::checkHierarchyValue() const {
    if (CurrentThreadValue <= mHierarchialValue) {
        throw std::logic_error("mutex hierarchy violation!");
    }
}

void HierarchialMutex::updateHierarchValue() {
    mPreviousValue = mHierarchialValue;
    // 修改current thread value为指定值
    CurrentThreadValue = mHierarchialValue;
}

/********************************更细粒度的锁*******************************/
class X {
    friend bool operator==(const X&, const X&);

public:
    explicit X(int);

private:
    int getDetails() const;

    int mValue;
    mutable std::mutex mMutex;
};

X::X(int v) : mValue(v) {}
int X::getDetails() const {
    std::unique_lock<std::mutex> lock(mMutex);
    return mValue;
}

bool operator==(const X& a, const X& b) {
    if (&a == &b) {
        return true;
    }
    int va = a.getDetails();
    int vb = b.getDetails();
    return va == vb;
}

};  // namespace mutex_ns

namespace other_tools {
//仅在初始化过程中保护共享数据,用于延迟初始化
std::once_flag flag;
void fun() {}
// once_flag和std::call_once保证了worker被多个线程使用时fun只会被调用一次
void worker() { std::call_once(flag, fun); }

};  // namespace other_tools

int main() {
    //

    return 0;
}