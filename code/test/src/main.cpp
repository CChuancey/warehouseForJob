#include <iostream>
#include <list>
#include <memory>

namespace observer {

// 抽象观察者，动作为更新状态、做出其他动作
class Observer {
public:
    virtual void update() = 0;
    virtual ~Observer() {}
};

class Obj {
public:
    //对象在某种情况下会通知观察者
    void notify() {
        for (auto observer : observers) {
            observer->update();
        }
    }

    void attach(Observer* observer) { observers.push_back(observer); }
    void detach(Observer* observer) { observers.remove(observer); }

    ~Obj() {
        for (auto observer : observers) {
            delete observer;
        }
    }

private:
    // 对于该类对象，存在的观察者
    std::list<Observer*> observers;
};

class TypeObserver : public Observer {
public:
    virtual void update() override {
        std::cout << "观察者A收到了信息" << std::endl;
    }
};

};  // namespace observer

namespace smartPointer {
// 对象中存储着观察者模式的对象指针，将其用weak_ptr代替
class Obj {
public:
    //对象在某种情况下会通知观察者
    void notify() {
        for (auto observer : observers) {
            // 检查指针是否有效
            auto obs = observer.lock();
            if (obs) {
                obs->update();
            }
        }
    }

    void attach(std::shared_ptr<observer::Observer>&& observer) {
        observers.push_back(observer);
    }
    // weak_ptr不存在operator==
    // void detach(const std::shared_ptr<observer::Observer>& observer) {
    //     std::weak_ptr<observer::Observer> wpr(observer);
    //     observers.remove(wpr);
    // }

private:
    std::list<std::weak_ptr<observer::Observer>> observers;
};
};  // namespace smartPointer

int main() {
    // 传统方式
    {
        auto obser = new observer::TypeObserver();
        auto obj = new observer::Obj();
        obj->attach(obser);
        obj->notify();
        delete obj;
    }

    // 智能指针替换裸指针
    {
        auto obser(std::make_shared<observer::TypeObserver>());
        auto obj(std::make_shared<smartPointer::Obj>());
        obj->attach(std::move(obser));
        obj->notify();
    }

    return 0;
}
