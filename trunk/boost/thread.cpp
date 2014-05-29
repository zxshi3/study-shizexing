/**
 * example to show boost thread library usage
 */
#include <boost/thread.hpp>
#include <iostream>
#include <cstdio>

class Worker {
public:
    Worker(const std::string && name) :
        _name(std::move(name)) {
        
    }
    void run() {
        for (int i = 0 ; i < 10; i++) {
            // http://www.boost.org/doc/libs/1_55_0/doc/html/chrono/reference.html#chrono.reference.cpp0x.duration_hpp
            //std::cout << _name << " sleep 1 seconds" << std::endl;
            boost::this_thread::sleep_for(boost::chrono::seconds(1));
            printf("[%d]\t%s sleep 1 seconds\n", i, _name.c_str());
        }
    }
private:
    std::string _name;
};

void testThreadGroup () {
    // http://stackoverflow.com/questions/19500404/how-to-create-a-thread-pool-using-boost-in-c
    boost::thread_group pool;
    Worker w1("thread 1");
    Worker w2("thread 2");
    pool.create_thread(boost::bind(&Worker::run, &w1));
    pool.create_thread(boost::bind(&Worker::run, &w2));
    std::cout << "join all workers" << std::endl;
    pool.join_all();
    std::cout << "join successful" << std::endl;
}

int main(int argc, char** argv) {
    testThreadGroup();
    return 0;
}