#include <iostream>
#include <list>
#include <memory>

namespace lazy {
// 懒汉模式，只有在需要时才创建,程序结束时无法释放
class SingleTon {
public:
    static SingleTon* getInstance() {
        if (!instance) {
            instance = new SingleTon();
        }
        return instance;
    }
    void printHello() {
        std::cout << "Hello lazy pattern!" << std::endl;
    }

private:
    SingleTon() = default;
    static SingleTon* instance;
};

SingleTon* SingleTon::instance = nullptr;
}; // namespace lazy

// C++11 起的最佳写法
namespace lazyAndThreadSafe {
class Singleton {
public:
    static Singleton& getInstance() {
        // C++11起静态局部变量线程安全
        static Singleton instance;
        return instance;
    }

public:
    // 禁止拷贝和移动
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;

public:
    void printHello() {
        std::cout << "Hello lazyAndThreadSafe pattern!" << std::endl;
    }

private:
    Singleton() = default;
    ~Singleton() = default;
};
}; // namespace lazyAndThreadSafe

namespace smartpointer {
class Singleton {
public:
    static Singleton& getInstance() {
        // static unique_ptr
        static std::unique_ptr<Singleton> instance(new Singleton);
        return *instance;
    }

public:
    void printHello() {
        std::cout << "Hello smartpointer pattern!" << std::endl;
    }

public:
    // 禁止拷贝和移动
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;
    // 析构函数需要public权限，供智能指针调用析构函数
    ~Singleton() = default;

private:
    Singleton() = default;

private:
    std::shared_ptr<Singleton> instance;
};
}; // namespace smartpointer

int main() {
    // 经典写法,存在内存泄漏
    {
        auto instance = lazy::SingleTon::getInstance();
        instance->printHello();
    }

    // C++11 起的最佳写法
    {
        auto& instance = lazyAndThreadSafe::Singleton::getInstance();
        instance.printHello();
    }
    // 利用智能指针但仍是静态局部变量
    {
        auto& instance = smartpointer::Singleton::getInstance();
        instance.printHello();
    }
    return 0;
}
