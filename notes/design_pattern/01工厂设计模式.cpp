#include <iostream>
#include <memory>

class Product {
public:
    virtual void info() = 0;
    virtual ~Product() {}
};

class TypeAProduct : public Product {
public:
    virtual void info() { std::cout << "TypeA product info" << std::endl; }
};

class TypeBProduct : public Product {
public:
    virtual void info() { std::cout << "TypeB product info" << std::endl; }
};

// 简单工厂模式，根据描述信息获得产品
namespace simple {
class Factory {
public:
    static Product* createProduct(int type) {
        if (type == 0) {
            return new TypeAProduct;
        } else {
            return new TypeBProduct;
        }
    }
};
};  // namespace simple

// 工厂方法类,将工厂进一步细分，每种工厂只生产某种特定的产品
namespace factoryFunction {
class Factory {
public:
    virtual Product* createProduct() = 0;
};

class TypeAFactory : public Factory {
public:
    virtual Product* createProduct() override { return new TypeAProduct; }
};

class TypeBFactory : public Factory {
public:
    virtual Product* createProduct() override { return new TypeBProduct; }
};
};  // namespace factoryFunction

namespace simpleBaseOnSmartPointer {
class Factory {
public:
    static std::unique_ptr<Product> createProduct() {
        static auto product = std::make_unique<TypeAProduct>();
        return std::move(product);
    }
};
};  // namespace simpleBaseOnSmartPointer

int main() {
    // 简单工厂设计模式
    {
        Product* product = simple::Factory::createProduct(1);
        delete product;
    }

    // 工厂方法设计模式
    {
        auto factory = new factoryFunction::TypeAFactory();
        auto typeAProduct = factory->createProduct();
        delete typeAProduct;
        delete factory;
    }

    // 使用智能指针实现的简单工厂模式
    {
        auto factory = std::make_unique<simpleBaseOnSmartPointer::Factory>();
        auto product(factory->createProduct());
        // convert from unique_ptr to shared_ptr
        std::shared_ptr<Product> copied_shared_ptr(std::move(product));
    }

    return 0;
}
