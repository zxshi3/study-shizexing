/**
 * this is a example to show smart_ptr
 */
#include <memory>
#include <iostream>
#include <cstdint>

class A {
public:
    A() {
        std::cout << "@" << __FUNCTION__ << std::endl;
    }
    virtual ~A() {
        std::cout << "@" << __FUNCTION__ << std::endl;
    }
};

class SmartPtrTester {
public:
    void testSharedPtr() {
        std::shared_ptr<A> pA = std::shared_ptr<A>(new A);
        // A* pA2 = new A;         // leak here
        std::unique_ptr<char[]> pca(new char[10]);
        std::shared_ptr<char> pc(new char[10], std::default_delete<char[]>());
    }
    void testUniquePtr() {
        std::string hello = "hello";
        std::unique_ptr<char[]> p(new char[hello.length() + 5]);
        char c = '\0';
        memcpy(p.get(), &c, sizeof(char));
        size_t n = hello.length();
        memcpy(p.get() + sizeof(char), &n, sizeof(char) * 4);
        memcpy(p.get() + 5, hello.c_str(), hello.length());
        std::string ss = std::string(p.get(), 5 + hello.length());
        std::cout << "ss.length = " << ss.length() << std::endl;
        std::cout << "ss = " << ss << std::endl;
        for (int i = 0; i < ss.length(); i++) {
            if (ss[i] >= 'A') {
                std::cout << "ss[" << i << "] = " << ss[i] << std::endl;
            } else {
                std::cout << "ss[" << i << "] = " << (int)ss[i] << std::endl;
            }
        }
        T t;
        t.t = 1;
        t.l = hello.length();
        std::cout << "sizeof(t)" << sizeof(t) << std::endl;
        std::string st((char*)&t.t, sizeof(t.t));
        std::string sl((char*)&t.l, sizeof(t.l));
        ss = st + sl + hello;
        std::cout << "ss.length = " << ss.length() << std::endl;
        std::cout << "ss = " << ss << std::endl;
        for (int i = 0; i < ss.length(); i++) {
            if (ss[i] >= 'A') {
                std::cout << "ss[" << i << "] = " << ss[i] << std::endl;
            } else {
                std::cout << "ss[" << i << "] = " << (int)ss[i] << std::endl;
            }
        }
    }
private:
    struct T {
        std::uint32_t l;
        std::uint8_t t;
    };
};

int main(int argc, char** argv) {
    SmartPtrTester tester;
    tester.testSharedPtr();
    tester.testUniquePtr();
    return 0;
}