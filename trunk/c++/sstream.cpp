/**
 * test std::stringstream
 */
#include <sstream>
#include <iostream>

namespace shi {
namespace zexing {
class stringstream {
};
}	// zexing
}	// shi

int main(int argc, char** argv) {
	std::stringstream ss;
	int lat, lon;
	lat = 3737490;
	lon = -12209876;
	//ss << "lat : " << lat << ", lon : " << lon;
	ss << lat << lon;
	std::string str(ss.str());
	std::cout << str << std::endl;
	int lat1, lon1;
	std::string delimlat, delimlon;
	//ss >> delimlat >> lat1 >> delimlon >> lon1;
	std::string sval = ss.str();
	std::stringstream ss2;
	ss2 << sval;
	ss2 >> lat1 >> lon1;
	std::cout << "lat1 = " << lat1 << ", lon1 = " << lon1 << std::endl;
	return 0;
}

