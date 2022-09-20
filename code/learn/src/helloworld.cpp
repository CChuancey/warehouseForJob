#include <algorithm>
#include <chrono>
#include <cstdint>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <vector>

struct MyStruct {
    MyStruct(int& _m) : m(_m) {}
    // v 是局部变量，线程还未结束时，v已经被销毁
    void operator()() {
        for (int64_t i = 0; i < INT64_MAX; i++) {
            i++;
            m++;
            std::cout << m << std::endl;
        }
    }

    int& m;
};

void testDetach() {
    int v = -1;
    MyStruct tester(v);
    std::thread t1(tester);
    t1.detach();
}

// 如果在线程中发生了异常&&等待线程结束，需要在其后调用join
// 如果线程可以不抛出异常&&等待线程结束，也需要调用join
void testJoin() {
    int v = -1;
    MyStruct tester(v);
    std::thread t2;

    // 这是一种防止内存泄漏发生的方式，还可以通过RAII，在析构函数调用join
    try {
        // 发生了异常
    } catch (...) {
        t2.join();
        throw;
    }
    t2.join();
}

/***************************Begin RAII*********************************/
// RAII的方式防止内存泄漏
class ThreadGuard {
    std::thread& mThread;

public:
    ThreadGuard(std::thread& t) : mThread(t) {}

    ~ThreadGuard() {
        if (mThread.joinable()) {
            mThread.join();
        }
    }

    // const ThreadGuard& 和 ThreadGuard const&等价
    ThreadGuard(const ThreadGuard&) = delete;
    ThreadGuard& operator=(ThreadGuard const&) = delete;
};

void testJoin2() {
    int v = -1;
    MyStruct tester(v);
    std::thread t3;
    ThreadGuard guard(t3);
    // do something
}

/***************************End RAII***********************************/

/****************************Start std::bind和std::ref的区别***********/
// std::bind
// 总是拷贝其参数，但是，调用者可以使用std::ref来实现传递引用给std::bind

void testFun(int& a, int& b, int& c) {
    a++;
    b++;
    c++;

    std::cout << a << " " << b << " " << c << std::endl;
}

void testBindAndRef() {
    int a = 1, b = 2, c = 3;
    int& d = b;

    // 由此可见bind在传递参数时是拷贝了一份变量；而std::ref生成了对c变量的引用，bind拷贝时拷贝的是引用
    // std::ref和普通的引用不同
    auto fun = std::bind(testFun, a, d, std::ref(c));
    fun();
    // thread 的传参如果函数需要引用，需要传入引用
    auto threadFun = [](int aa, int& bb, int&& cc) {
        std::cout << aa << " " << bb << " " << cc << std::endl;
    };
    std::thread testThread(threadFun, a, std::ref(b), std::move(c));
    testThread.join();
    std::cout << "after testFunc" << a << " " << b << " " << c << std::endl;
}

/****************************End std::bind和std::ref的区别*************/

// 测试thread在线程中拷贝
void testThreadCopy(int& x) {
    x++;
    std::cout << "testThreadCopy" << x << std::endl;
}

// 测试thread使用成员函数
class TestMemberFunc {
public:
    void do_work(int v) { std::cout << v << std::endl; }
};

void testMemberFuncFun() {
    TestMemberFunc tester;
    int v = 0;
    // 不使用std::ref也可以成功拷贝进线程空间？
    std::thread t4{&TestMemberFunc::do_work, &tester, v};
    t4.join();
}

// 移动构造函数、移动赋值运算符
class Tmp {
public:
    Tmp(const Tmp&);
    // 移动构造函数
    Tmp(const Tmp&&) noexcept;
    // 移动赋值运算符拷贝
    Tmp& operator=(const Tmp&& oth) noexcept;
};

// 确保线程结束前完成join的操作
class ScopedThread {
public:
    ScopedThread(std::thread t) : mThread(std::move(t)) {
        if (!mThread.joinable()) {
            throw std::logic_error("No Thread");
        }
    }

    ~ScopedThread() { mThread.join(); }

    // 禁止拷贝和赋值
    ScopedThread(const ScopedThread&) = delete;
    ScopedThread& operator=(const ScopedThread&) = delete;

private:
    std::thread mThread;
};

int main(int argc, char** argv) {
    std::cout << "hello world" << std::endl;

    // C++标准库只管理与std::thread相关联的线程
    // C++11
    // 风格定义类对象，应使用列表初始化的形式；避免出现二义性，将其解释为函数声明
    std::thread t{[]() { std::cout << "hello thread" << std::endl; }};

    // 对于join模式，主线程需要等待t线程结束才会结束，t线程结束后会对其进行清理操作
    // 对于detach模式，主线程不需要等待t线程结束
    // ==》线程对象如何管理的？
    t.join();

    // testDetach();
    // testJoin();
    testJoin2();

    // 测试detach的线程能否joinable
    std::thread t2{[]() {
        std::cout << "detached" << std::endl;
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(2000ms);
    }};
    t2.detach();
    if (t2.joinable()) {
        std::cout << "t2 joinable" << std::endl;
    }

    // 向thread构造函数中传递线程函数运行所需要的参数
    std::thread t3{[&](const std::string& s) { std::cout << s << std::endl; },
                   "lambda thread paramters!"};
    t3.join();

    // 测试bind和ref的区别
    testBindAndRef();

    //线程函数的参数按值移动或复制。如果引用参数需要传递给线程函数，它必须被包装（例如使用std
    //:: ref或std :: cref）
    int v = 1;
    std::thread t4{[=](int& x) {
                       x++;
                       std::cout << "x:" << x << std::endl;
                   },
                   std::ref(v)};
    t4.join();
    std::cout << "after ++:" << v << std::endl;

    // 传递的参数不加ref编译过不了
    std::thread t5{testThreadCopy, std::ref(v)};
    t5.join();
    std::cout << "after ++:" << v << std::endl;

    // 线程执行函数还可以是某个成员函数，此时std::thread的参数分布情况如下：
    // 0：成员函数地址
    // 1. 成员函数所属对象的地址
    // 2. 成员函数的参数
    testMemberFuncFun();
    // auto test = new ThreadGuard(t4);

    // 不能通过赋新值的方式让一个std::thread对象“丢弃”一个线程
    // {
    //     auto someFunction = [] {};
    //     auto someOtherFunction = [] {};
    //     std::thread t1(someFunction);         // 1
    //     std::thread t2 = std::move(t1);       // 2
    //     t1 = std::thread(someOtherFunction);  // 3
    //     std::thread t3;                       // 4
    //     t3 = std::move(t2);                   // 5
    //     // t1 = std::move(t3);  // 6 赋值操作将使程序崩溃
    // }

    // 确保线程结束前完成join的操作
    {
        auto fun = [] {};
        ScopedThread thread(std::move(std::thread(fun)));
    }

    // 创建一组线程进行管理
    {
        auto work = [](int i) {
            std::cout << "Thread " << i << " is working!" << std::endl;
        };
        std::vector<std::thread> threads;
        const int placeholder = 20;
        for (int i; i < placeholder; i++) {
            threads.emplace_back(work, i);
        }

        for (auto& t : threads) {
            t.join();
        }
    }

    // 线程数量
    {
        // 多核系统中返回值为CPU核心数量，如果无法获取返回0
        auto num = std::thread::hardware_concurrency();
        std::cout << "CPU core num :" << num << std::endl;
    }

    // 线程id
    {
        // 1. 通过std::this_thread::get_id()
        std::thread t(
            [] { std::cout << std::this_thread::get_id() << std::endl; });

        // 2. 通过thread对象的成员函数
        std::cout << t.get_id() << std::endl;
        t.join();
    }

    return 0;
}