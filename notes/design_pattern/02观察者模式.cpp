#include <iostream>
#include <list>

namespace observer {

// 抽象观察者，动作为更新状态、做出其他动作
class Observer {
public:
    virtual void update() = 0;
    virtual ~Observer() {
    }
};

class Obj {
public:
    //对象在某种情况下会通知观察者
    void notify() {
        for (auto observer : observers) {
            observer->update();
        }
    }

    void attach(Observer* observer) {
        observers.push_back(observer);
    }
    void detach(Observer* observer) {
        observers.remove(observer);
    }

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

}; // namespace observer

int main() {
    auto obser = new observer::TypeObserver();
    auto obj = new observer::Obj();
    obj->attach(obser);
    obj->notify();
    delete obj;

    return 0;
}
