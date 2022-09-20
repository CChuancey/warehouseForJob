#include <iostream>
#include <list>
#include <memory>

namespace adapter {
class Target {
public:
    virtual void execute() = 0;
};

class Adaptee {
public:
    static void doAdapte() { std::cout << "adaptee !" << std::endl; }
};

class Adapter : public Target {
public:
    void execute() override { mAdaptee->doAdapte(); }

private:
    std::unique_ptr<Adaptee> mAdaptee;
};

};  // namespace adapter

int main() {
    std::shared_ptr<adapter::Target> target(
        std::make_shared<adapter::Adapter>());
    target->execute();
    return 0;
}
