#include <iostream>
#include <typeinfo>
#include <map>
#include <memory>
#include <string>
#include <boost/shared_ptr.hpp>

class Base {
public:
	virtual ~Base() {};
	virtual void handle(const void*) = 0;
};

template <class EventT, class T>
class Handler : public Base {
public:
	explicit Handler(T & h, void (T::*func)(const EventT&)) :
		_h(h),
		_func(func) {
	}
	void handle(const void* p) {
		std::cout << std::endl << "@handle(void *p)" << std::endl;
		std::cout << "typeid(p) : " << typeid(p).name() << std::endl;
		const EventT* pe = static_cast<const EventT*>(p);
		std::cout << "typeid(*pe) : " << typeid(*pe).name() << std::endl;
		(_h.*_func)(*pe);
	}
private:
	friend class Bus;
	T & _h;
	void (T::*_func)(const EventT&);
};

class Bus {
public:
	template<class EventT, class T>
	void registerEvent(T & h, void (T::*func)(const EventT&)) {
		std::cout << std::endl << "@registerEvent" << std::endl;
		std::cout << "typeid(T) : " << typeid(T).name() << std::endl;
		std::cout << "typeid(EventT) : " << typeid(EventT).name() << std::endl;
		std::cout << "typeid(h) : " << typeid(h).name() << std::endl;
		std::cout << "typeid(func) : " << typeid(func).name() << std::endl;
		boost::shared_ptr<Handler<EventT, T> > pHandler = boost::shared_ptr<Handler<EventT, T> >(new Handler<EventT, T>(h, func));
		boost::shared_ptr<Base> pBase = boost::static_pointer_cast<Base>(pHandler);
//		handlers.insert(std::pair<std::type_info, boost::shared_ptr<Base> >(typeid(EventT), pBase));		// std::type_info do not have operator<
		handlers.insert(std::pair<const std::string, boost::shared_ptr<Base> >(typeid(EventT).name(), pBase));
	}

	template<class EventT, class T>
	void unregisterEvent(T & h, void (T::*func)(const EventT&)) {
		std::cout << std::endl << "@unregisterEvent" << std::endl;
		std::cout << "typeid(T) : " << typeid(T).name() << std::endl;
		std::cout << "typeid(EventT) : " << typeid(EventT).name() << std::endl;
		std::cout << "typeid(h) : " << typeid(h).name() << std::endl;
		std::cout << "typeid(func) : " << typeid(func).name() << std::endl;
//		std::pair<std::multimap<std::type_info, boost::shared_ptr<Base> >::iterator, std::multimap<std::type_info, boost::shared_ptr<Base> >::iterator> matches = handlers.equal_range(typeid(EventT));
		std::pair<std::multimap<std::string, boost::shared_ptr<Base> >::iterator, std::multimap<std::string, boost::shared_ptr<Base> >::iterator> matches = handlers.equal_range(typeid(EventT).name());
//		for (std::multimap<std::type_info, boost::shared_ptr<Base> >::iterator it = matches.first; it != matches.second; it++) {
		for (std::multimap<std::string, boost::shared_ptr<Base> >::iterator it = matches.first; it != matches.second; it++) {
			boost::shared_ptr<Handler<EventT, T> > pHandler = boost::static_pointer_cast<Handler<EventT, T> >(it->second);
			if (func == pHandler->_func) {
				handlers.erase(it);
				std::cout << "remove " << typeid(func).name() << " successful" << std::endl;
				return;
			}
		}
	}

	template<class EventT>
	void broadcast(const EventT& e) {
//		std::pair<std::multimap<std::type_info, boost::shared_ptr<Base> >::iterator, std::multimap<std::type_info, boost::shared_ptr<Base> >::iterator> matches = handlers.equal_range(typeid(EventT));
		std::pair<std::multimap<std::string, boost::shared_ptr<Base> >::iterator, std::multimap<std::string, boost::shared_ptr<Base> >::iterator> matches = handlers.equal_range(typeid(EventT).name());
//		for (std::multimap<std::type_info, boost::shared_ptr<Base> >::iterator it = matches.first; it != matches.second; it++) {
		for (std::multimap<std::string, boost::shared_ptr<Base> >::iterator it = matches.first; it != matches.second; it++) {
			// do not know class T
			//boost::shared_ptr<Handler<EventT, T> > pHandler = boost::static_pointer_cast<Handler<EventT, T> >(it->second);
			//pHandler->handle(e);
			it->second->handle(&e);
		}
	}
private:
//	std::multimap<std::type_info, boost::shared_ptr<Base> > handlers;
	std::multimap<std::string, boost::shared_ptr<Base> > handlers;
};

class E1 {};
class E2 {};
class E3 {};

template <class EventT>
class EventAdapter : public EventT {
};

template <class EventT, class H>
class HandlerAdapter {
};


class H1 {
public:
	void handle(const E1& e) {
		std::cout << "H1 handle E1" << std::endl;
	}
	void handle(const E2& e) {
		std::cout << "H1 handle E2" << std::endl;
	}
};

class H2 {
public:
	void handle(const E2& e) {
		std::cout << "H2 handle E2" << std::endl;
	}
	void handle(const E3& e) {
		std::cout << "H2 handle E3" << std::endl;
	}
};

int main(int argc, char** argv) {
	H1 h1;
	H2 h2;

	Bus bus;

	bus.registerEvent<E1, H1>(h1, static_cast<void (H1::*)(const E1&)>(&H1::handle));
	bus.registerEvent<E2, H1>(h1, static_cast<void (H1::*)(const E2&)>(&H1::handle));
	bus.registerEvent<E2, H2>(h2, static_cast<void (H2::*)(const E2&)>(&H2::handle));

	int a,b;
	int arr[] = {1, 3};
	E1 e1;
	bus.broadcast<E1>(e1);
	E2 e2;
	bus.broadcast<E2>(e2);

	EventAdapter<E1> e1a;
	bus.broadcast<EventAdapter<E1> >(e1a);

	bus.unregisterEvent<E1, H1>(h1, static_cast<void (H1::*)(const E1&)>(&H1::handle));
	bus.unregisterEvent<E2, H1>(h1, static_cast<void (H1::*)(const E2&)>(&H1::handle));
	bus.unregisterEvent<E2, H2>(h2, static_cast<void (H2::*)(const E2&)>(&H2::handle));
	bus.unregisterEvent<E3, H2>(h2, static_cast<void (H2::*)(const E3&)>(&H2::handle));

	return 0;
}

