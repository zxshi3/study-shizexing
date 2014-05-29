#include <boost/smart_ptr.hpp>
#include <iostream>
#include <map>

class Handler {
public:
    virtual ~Handler() {}
    virtual void handle(const std::string & data) {}
};

class T : public boost::enable_shared_from_this<T> {
public:
    T() :
        boost::enable_shared_from_this<T>() {
        // make_share();        // shared_from_this() will cause exception in construtor
    }
    boost::shared_ptr<T> make_share() {
        return shared_from_this();
    }
    ~T() {}
private:
    std::multimap<int, boost::weak_ptr<Handler> > mp;
};

int main(int argc, char** argv) {
    boost::weak_ptr<char> wp;
    
    boost::shared_ptr<char> t = wp.lock();
    assert(t.get() == NULL);
    boost::shared_ptr<char> p(new char('a'));
    wp = boost::weak_ptr<char>(p);
    t = wp.lock();
    std::cout << "*t = " << *t << std::endl;
    
    boost::shared_ptr<T> c(new T);
    std::cout << "ok" << std::endl;
    c->make_share();
    std::cout << "ok2" << std::endl;
    return 0;
}