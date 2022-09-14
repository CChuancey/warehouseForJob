#include <cstddef>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

// 普通版本的类，正常的拷贝构造、拷贝赋值、析构
namespace common {
class HasPtr {
public:
    HasPtr(std::string* s = new std::string()) :
        str(s), value(0) {
    }
    HasPtr(const HasPtr& oth) {
        value = oth.value;
        str = new std::string(*oth.str);
    }

    HasPtr& operator=(const HasPtr& oth) {
        if (&oth == this) {
            return *this;
        }
        HasPtr tmp(oth);
        std::swap(tmp, *this);
        return *this;
    }

    ~HasPtr() {
        delete str;
    }

private:
    int value;
    std::string* str;
};
} // namespace common

namespace pointer {

// 简单的智能指针
class HasPtr {
public:
    HasPtr(const std::string& s = std::string()) :
        str(new std::string(s)), value(0), count(new size_t) {
        *count = 1;
    }

    HasPtr(const HasPtr& oth) {
        str = oth.str;
        value = oth.value;
        count = oth.count;
        ++(*count);
    }

    HasPtr& operator=(const HasPtr& oth) {
        HasPtr tmp(oth);
        swap(tmp);

        // 使用std::swap会产生内存泄漏
        // HasPtr tmp(oth);
        // std::swap(tmp, *this);
        // return *this;

        return *this;
    }

    ~HasPtr() {
        if (--(*this->count) == 0) {
            // std::cout << "freeed" << std::endl;
            delete str;
            delete count;
        }
    }

    int getCount() const {
        return *count;
    }

    std::string operator*() {
        return *str;
    }

    // operator -> 需要另行考虑

private:
    std::string* str;
    int value;
    size_t* count;

    void swap(HasPtr& oth) {
        std::swap(oth.str, str);
        std::swap(oth.value, value);
        std::swap(oth.count, count);
    }
};

}; // namespace pointer

int f() {
    return 1;
}

namespace rvalue {
/**
 * @brief 对于C++11，如果定义了析构函数，编译器不会自动生成拷贝构造函数、移动构造函数。
 *      默认的移动构造函数，只有在没有声明析构函数、拷贝构造函数、移动构造函数时才会声明;
 *      一旦声明移动构造函数，编译器便不会自己生成拷贝构造函数;
 * 
 *  更新版三五法则：如果定义了拷贝操作，就应该定义所有五个操作
 *
 */
class Tester {
public:
    Tester(int* hh = new int(10)) :
        p(hh) {
    }
    ~Tester() {
        if (p) {
            delete p;
        }
    }

    Tester(const Tester& oth) {
        p = new int(*(oth.p));
    }

    Tester& operator=(const Tester& oth) {
        Tester tmp(oth);
        swap(tmp);
        return *this;
    }

    Tester(Tester&& oth) noexcept :
        p(oth.p) {
        oth.p = nullptr;
    }

    // Tester& operator=(Tester&& oth) noexcept {
    //     if (this != &oth) {
    //         if (p) { //删除自身资源
    //             delete p;
    //             p = nullptr;
    //         }
    //         p = oth.p;
    //         oth.p = nullptr;
    //     }
    //     return *this;
    // }

    // 显式声明移动运算符让编译器自动生成即可满足需求
    Tester& operator=(Tester&& oth) noexcept = default;

    void swap(Tester& oth) {
        std::swap(oth.p, p);
    }

    void printValue() const {
        std::cout << *p << std::endl;
    }

private:
    int* p;
};

struct hasY {
    hasY() = default;
    hasY(hasY&&) = default;
    int v;
};

hasY x;
hasY y(std::move(x));

}; // namespace rvalue

int main(int argc, char** argv) {
    pointer::HasPtr p;
    std::cout << p.getCount() << std::endl;
    // pointer::HasPtr p1 = p;
    // std::cout << p1.getCount() << std::endl;

    rvalue::Tester test;
    test.printValue();
    rvalue::Tester cpTest = std::move(test);

    return 0;
}