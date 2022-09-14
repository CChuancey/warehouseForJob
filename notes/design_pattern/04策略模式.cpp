#include <iostream>
#include <list>
#include <memory>

namespace strategy {
class Strategy {
public:
    virtual ~Strategy() = default;
    virtual void run() = 0;
    // 默认构造函数也需要声明，后面用到了无参构造函数
    Strategy() = default;
    Strategy(const Strategy&) = default;
    Strategy& operator=(const Strategy&) = default;
    Strategy& operator=(Strategy&&) = default;
    Strategy(Strategy&&) = default;
};

class StragtegyTypeA : public Strategy {
public:
    void run() override { std::cout << "Type A Strategy running" << std::endl; }
};

class StragtegyTypeB : public Strategy {
public:
    void run() override { std::cout << "Type B Strategy running" << std::endl; }
};

class Worker {
public:
    explicit Worker(std::unique_ptr<Strategy> strate)
        : mStrategy(std::move(strate)) {}
    void doJobs() {
        std::cout << "worker 正在干活" << std::endl;
        mStrategy->run();
    }

private:
    std::unique_ptr<Strategy> mStrategy;
};

}  // namespace strategy

int main() {
    auto worker(std::make_shared<strategy::Worker>(
        std::make_unique<strategy::StragtegyTypeA>()));
    worker->doJobs();
    return 0;
}
