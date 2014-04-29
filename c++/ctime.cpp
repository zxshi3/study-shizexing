/**
 * This is a test application to show how to use ctime
 * 1. calculate seconds different between local time and global time (UTC)
 *		diffLocalToGlobal();
 * 2. test gmtime
 *		test_gmtime();
 */
#include <stdlib.h>
#include <iostream>
#include <ctime>
// #include <cstring>

void test_gmtime() {
	std::cout << "====== @ test gmtime ======\n";
	time_t t = -1;
	tm * p = gmtime(&t);
	if (p) {
		char buf[256];
		size_t c = strftime(buf, sizeof(buf), "%a, %d %b %Y %X GMT", p);
		std::cout << "time_t(-1) : " << buf << std::endl;
	}
	std::cout << "====== # test gmtime ======" << std::endl;
}

/**
 * this function is used to calculate how many seconds between local time and global time (UTC)
 * return diff in seconds
 */
int diffLocalToGlobal();

void test() {
    struct tm gtm;
    gtm.tm_year = 2013 - 1900;
    //gtm.tm_mon = 8 - 1;
    gtm.tm_mon = 1 - 1;
    gtm.tm_mday = 2;
    gtm.tm_hour = 20;
    gtm.tm_min = 35;
    gtm.tm_sec = 3;
    //gtm.tm_isdst = 1;
    std::cout << "global time : " << asctime(&gtm);
    
    //time_t lt = timelocal(&gtm);
    //std::cout << "local time : " << ctime(&lt);
    
    time_t gt = timegm(&gtm);
    std::cout << "local time2 : " << ctime(&gt);
    
    {
    char * tz = getenv("TZ");
    setenv("TZ", "", 1);
    tzset();
    time_t t = mktime(&gtm);
    if (tz) {
        std::cout << "current time zone : " << tz << std::endl;
        setenv("TZ", tz, 1);
    }
    else
        unsetenv("TZ");
    tzset();
    std::cout << "local time : " << ctime(&t);
    }
}

void test2() {
    std::cout << "\ntest2\n";
    struct tm ltm;
    ltm.tm_year = 2013 - 1900;
    //ltm.tm_mon = 8 - 1;
    ltm.tm_mon = 1 - 1;
    ltm.tm_mday = 2;
    //ltm.tm_hour = 13;
    ltm.tm_hour = 12;
    ltm.tm_min = 35;
    ltm.tm_sec = 3;
    //ltm.tm_isdst = 1;
    std::cout << "local time : " << asctime(&ltm);
    
    time_t lt = timelocal(&ltm);
    std::cout << "local time : " << ctime(&lt);
    tm gtm = *gmtime(&lt);
    std::cout << "global time : " << asctime(&gtm);
    
    //time_t gt = timegm(&ltm);
    //std::cout << "global time2 : " << ctime(&gt);
}

int main (int argc, char** argv) {
    //int ds = diffLocalToGlobal();    
    test();
    test2();
    
    std::cout << std::endl;
    test_gmtime();
    std::cout << std::endl;
    
    return 0;
}

int diffLocalToGlobal() {
    time_t t = time(0);
    std::cout << "t = " << t << std::endl;
    tm ltm, gtm;
    ltm = *localtime(&t);
    std::cout << "local time : " << asctime(&ltm);
    if (ltm.tm_isdst) {
        std::cout << "\t is day light saving\n";
    }
    gtm = *gmtime(&t);
    std::cout << "global time : " << asctime(&gtm);
    time_t lt = mktime(&ltm);
    std::cout << "lt : " << ctime(&lt);
    time_t gt = mktime(&gtm);
    std::cout << "gt : " << ctime(&gt);
    std::cout << "diff hour : " << ((lt - gt) / 3600) << std::endl;
    
    {
        time_t lt2 = timelocal(&gtm);
        std::cout << "\nlt2 : " << ctime(&lt2);
        std::cout << "lt2 = " << lt2 << std::endl;
        time_t gt2 = timegm(&gtm);
        std::cout << "gt2 : " << ctime(&gt2);
        std::cout << "gt2 = " << gt2 << std::endl;
        std::cout << "t : " << ((t - gt2) / 3600) << std::endl;
        std::cout << "lt2 - gt2 : " << ((lt2 - gt2) / 3600) << std::endl;
    }
    
    return 0;
}


