#include <iostream>

class Base {
private:
    class FinalC {};
public:
    virtual void print(FinalC clz = FinalC()) /*sealed*/ {
        std::cout << "base\n";
    }
};

class Derived : public Base {
public:
    /*virtual */void print(Base::FinalC clz = Base::FinalC()) {
        std::cout << "derived\n";
    }
};

int main(int argc, char ** argv) {
    Base * pB = new Derived;
    pB->Base::print();
    pB->print();
    delete pB;
    return 0;
}
