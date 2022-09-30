#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>

namespace conditional {

using data_chunk = int;

// std::condition_variable 仅限于和std::mutex一起使用
std::mutex mut;
std::queue<data_chunk> dataQueue;
std::condition_variable dataCond;

void fun1() {
    data_chunk v = 1;

    // 将std::lock_guard放在语句块中，方便销毁
    {
        std::lock_guard<std::mutex> lk(mut);
        dataQueue.push(v);
    }
    // 此时mut已经解锁
    dataCond.notify_one();
}

void fun2() {
    std::unique_lock<std::mutex> lk(mut);
    // wait返回时会对mut加锁
    dataCond.wait(lk, [] { return !dataQueue.empty(); });

    data_chunk data = dataQueue.front();
    dataQueue.pop();

    lk.unlock();
    // process data
}

// 利用std::condition_variable 实现线程安全的队列
namespace safe_queue {
template <typename T>
class SafeQueue {
public:
    SafeQueue();
    SafeQueue(const SafeQueue&);
    //禁止拷贝
    SafeQueue& operator=(const SafeQueue&) = delete;

    void push(T);
    bool tryPop(T&);
    std::shared_ptr<T> tryPop();
    void waitAndPop(T&);
    std::shared_ptr<T> waitAndPop();
    bool empty() const;

private:
    std::queue<T> mQueue;
    mutable std::mutex mMutex;
    std::condition_variable mConditionV;
};
};  // namespace safe_queue

template <typename T>
safe_queue::SafeQueue<T>::SafeQueue() = default;

template <typename T>
safe_queue::SafeQueue<T>::SafeQueue(const SafeQueue<T>& oth) {
    // 赋值时先锁住
    std::lock_guard<std::mutex> lock(oth.mMutex);
    mQueue = oth.mQueue;
}

template <typename T>
void safe_queue::SafeQueue<T>::push(T v) {
    // 是否应该将lock写在块中？
    std::lock_guard<std::mutex> lock(mMutex);
    mQueue.push(v);

    mConditionV.notify_one();
}

template <typename T>
bool safe_queue::SafeQueue<T>::tryPop(T& v) {
    std::lock_guard<std::mutex> lock(mMutex);
    if (mQueue.empty()) {
        return false;
    }

    v = mQueue.front();
    mQueue.pop();
    // mConditionV.notify_one();

    return true;
}

template <typename T>
void safe_queue::SafeQueue<T>::waitAndPop(T& v) {
    std::unique_lock<std::mutex> lock(mMutex);
    // 必须在lambda中显式捕捉this
    dataCond.wait(mMutex, [this] { return !mQueue.empty(); });
    v = mQueue.front();
    mQueue.pop();
    mMutex.unlock();
}

template <typename T>
std::shared_ptr<T> safe_queue::SafeQueue<T>::waitAndPop() {
    std::unique_lock<std::mutex> lock(mMutex);
    dataCond.wait(mMutex, [this] { return !mQueue.empty(); });
    auto res(std::make_shared<T>(mQueue.front()));
    mQueue.pop();
    // lock.unlock();
    return res;
}

template <typename T>
bool safe_queue::SafeQueue<T>::empty() const {
    std::lock_guard<std::mutex> lock(mMutex);
    return mQueue.empty();
}

};  // namespace conditional

int main() {
    //
    return 0;
}