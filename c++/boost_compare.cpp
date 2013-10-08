#include <queue>
#include <boost/shared_ptr.hpp>

    template <class T>
    class Compare {
    public:
        bool operator() (const boost::shared_ptr<T> & lhs, const boost::shared_ptr<T> & rhs) {
            return *lhs < *rhs;
        }
    };

int main(int argc, char ** argv) {
    std::priority_queue<boost::shared_ptr<int> > q;
    std::priority_queue<boost::shared_ptr<int>, std::vector<boost::shared_ptr<int> >, Compare<int> > q2;
    q.push(boost::shared_ptr<int>(new int(10)));
    q.push(boost::shared_ptr<int>(new int(3)));
    q.push(boost::shared_ptr<int>(new int(20)));
    q.push(boost::shared_ptr<int>(new int(-9)));
    q.push(boost::shared_ptr<int>(new int(1)));

    while(!q.empty()) {
        boost::shared_ptr<int> p = q.top();
        q.pop();
        std::cout << *p << '\t';
    }
    std::cout << std::endl;

    q2.push(boost::shared_ptr<int>(new int(10)));
    q2.push(boost::shared_ptr<int>(new int(3)));
    q2.push(boost::shared_ptr<int>(new int(20)));
    q2.push(boost::shared_ptr<int>(new int(-9)));
    q2.push(boost::shared_ptr<int>(new int(1)));

    while(!q2.empty()) {
        boost::shared_ptr<int> p = q2.top();
        q2.pop();
        std::cout << *p << '\t';
    }
    std::cout << std::endl;

    return 0;
}
