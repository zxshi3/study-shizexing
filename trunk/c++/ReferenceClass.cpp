#include "ReferenceClass.h"
#include <string>
#include <iostream>

namespace zexing {
class RefClass {
public:
    std::string str;
};
}

void test() {
    zexing::RefClass ref;
    ref.str = "RefClass";
    print(ref);
}

void print(zexing::RefClass & obj) {
    std::cout << obj.str << std::endl;
}

