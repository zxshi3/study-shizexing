#include <iostream>
#include <typeinfo>

class Event {};
class EH {};

template <class EventT, class T>
class Handler {
public:
	Handler(T * p) : m_p(p) {};
	void handle(const EventT& e) {
		std::cout << "handle " << typeid(e).name() << std::endl;
	}
	template <class E, class H>
	bool isSameHandler(H & h) {
		std::cout << "typeid(*this).name() = " << typeid(*this).name() << std::endl;
		std::cout << "typeid(h).name() = " << typeid(h).name() << std::endl;
		return typeid(*m_p).name() == typeid(h).name() && typeid(E).name() == typeid(EventT).name();
	}
	T * m_p;
};

int main(int argc, char** argv) {
	EH eh;
	Handler<Event, EH> h1(&eh);
	Handler<Event, EH> h2(&eh);
	h1.isSameHandler<Event, EH>(eh);
	return 0;
}

