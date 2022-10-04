#include <sys/wait.h>

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <deque>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

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

namespace future {
// future同样分为独享型std::future和共享型std::shared_future
// 异步操作：std::async函数模板返回std::future
// 凭借std::sync即可将任务拆分为多个子任务并发运行

class MoveOnly {
public:
    explicit MoveOnly(int v) : mValue(v) {}
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;
    MoveOnly(MoveOnly&&) = default;
    MoveOnly& operator=(MoveOnly&&) = default;

    void fun(int v) const { std::cout << v + mValue << std::endl; }

private:
    int mValue;
};

MoveOnly test(1);

// 调用std::async时，可以指定启动方式。采用的是std::launch里面的enum class
// std::async的参数类似std::thread，依次传入函数、[对象]、参数
auto f1 = std::async(std::launch::async, &MoveOnly::fun, &test, 1);
auto f2 = std::async(std::launch::deferred, &MoveOnly::fun, &test, 2);
auto f3 = std::async(std::launch::async | std::launch::deferred, &MoveOnly::fun,
                     &test, 3);

void fun() {
    std::deque<int> dqi{1, 2, 3, 4, 5, 6, 7};
    int v(std::move(dqi.front()));
    dqi.pop_front();
    std::cout << dqi.size() << std::endl;
}

};  // namespace future

namespace scoped_task {

int func() {
    std::cout << "hello func work!" << std::endl;
    return INT32_MAX;
}

std::future<int> testTask() {
    // 构造task
    std::packaged_task<int()> task(func);
    // 执行task
    task();
    return task.get_future();
}

// std::promise和std::future是一对.
void testPromiseAndFuture() {
    std::promise<int> promis;
    std::future<int> fut = promis.get_future();

    auto f = std::async(std::launch::async, [&promis] {
        promis.set_value(-2);
        std::cout << "set!" << std::endl;
    });

    fut.wait();
    std::cout << fut.get() << std::endl;
}

// std::future是独占的，不可拷贝,std::std::shared_future为共享的
// 生成std::shared_future的方式]
std::promise<int> pro;
std::future<int> fu = pro.get_future();
// 采用显示的移动语义构造
// std::shared_future<int> sfu1 = std::move(fu);

// // 采用隐式 移动构造
// std::shared_future<int> sfu2(pro.get_future());

// // 采用future的方法构造
// std::shared_future<int> sfu3 = fu.share();

}  // namespace scoped_task

// C++中的时钟类
namespace time_clock {
// 四项关键信息
// 1.当前时刻
// 2.时间值的类型
// 3.该时钟单元的计时类型
// 4.计时速率

void testFunction() {
    // 当前时刻
    std::cout << std::chrono::system_clock::now().time_since_epoch().count()
              << std::endl;

    // 时钟--超时的用法后面再补
    using namespace std::chrono_literals;
    auto oneday = 24h;
    auto halfAnHour = 30min;
    auto maxTimeBetweenMessages = 30ms;
}

};  // namespace time_clock

// 函数式编程可以简化并发操作-------------------->后面再补
// 实例：多线程完成快排

int main() {
    // test future
    future::f1.get();
    future::f2.get();
    future::f3.get();
    future::fun();

    // test packaged_task
    std::cout << scoped_task::testTask().get() << std::endl;

    // test promise and future
    scoped_task::testPromiseAndFuture();

    return 0;
}