#include <iostream>

class A {
public:
    A() 
        : x(0)
        , y(0) 
        , z(1)
        , b(true)
    {
    }

    void print() {
        std::cout << "x = " << x << ", y = " << y << std::endl;
    }
private:
    bool b;
    int z;
    int x;
    int y;
};

int main(int argc, char** argv) {
    A a;
    a.print();
    return 0;
}

