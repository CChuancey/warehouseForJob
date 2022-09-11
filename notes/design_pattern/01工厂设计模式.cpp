#include <iostream>

class Product {
public:
    virtual void create();
};

class TypeAProduct : public Product {
public:
    virtual void create() {
        std::cout << "create TypeA product" << std::endl;
    }
};

class TypeBProduct : public Product {
public:
    virtual void create() {
        std::cout << "create TypeB product" << std::endl;
    }
};

class Factory {
public:
    static Product *createProduct(int type) {
        if (type == 0) {
            return new TypeAProduct;
        } else {
            return new TypeBProduct;
        }
    }
};

int main() {
    Product *product = Factory::createProduct(1);
    delete product;
    return 0;
}
