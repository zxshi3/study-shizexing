#include <ctime>
#include <iostream>

int main(int argc, char** argv) {
	struct tm tm0 = {0};
	time_t t0;
	struct tm * ptm;
	t0 = 0;
	ptm = gmtime(&t0);
	std::cout << asctime(ptm) << std::endl;
	tm0.tm_year = 1970 - 1900;
	// tm0.tm_mon = 0;
	tm0.tm_mday = 1;
//	tm0.tm_isdst = 1;
	t0 = mktime(&tm0);
	std::cout << "t0 = " << t0 << std::endl;
	std::cout << "t0 / 3600 = " << (t0 / 3600) << std::endl;
	ptm = gmtime(&t0);
	std::cout << asctime(ptm) << std::endl;
	time_t tcur = time(NULL);
	ptm = gmtime(&tcur);
	std::cout << asctime(ptm) << std::endl;
	tcur = time(NULL) - t0;
	ptm = gmtime(&tcur);
	std::cout << asctime(ptm) << std::endl;
	
	time_t tsj = 1398805;
	ptm = gmtime(&tsj);
	std::cout << asctime(ptm) << std::endl;
	return 0;
}