#include <string>	// using std::string
#include <iostream>	// using std::cout/cerr/end/hex
//#include <sys/stat.h>
#include <fcntl.h>	// using open, O_RDONLY
#include <errno.h>	// using errno
#include <cstring>	// using ::strerror
#include <linux/input.h>	/* kernel input subsystem
	using struct input_event */
//#include <cstdio>	// using printf
#include <cstdlib>	// exit

/*
 * 1. old way, parsing from /dev/input/mouse2
 * 2. new way, parsing from /dev/input/event4
 */

int main(int argc, char** argv)
{
	std::cout << "parsing mouse input" << std::endl;
	//int fdmice = open("/dev/input/mice", O_RDWR | O_NONBLOCK);
	//int fdmice = open("/dev/input/mice", O_RDONLY | O_NONBLOCK);
	//int fdmice = open("/dev/input/mouse2", O_RDONLY);
	int fdmice = open("/dev/input/event4", O_RDONLY);
	if (fdmice == -1)
	{
		//std::cerr << "open /dev/input/mouse2 error. ";
		std::cerr << "open /dev/input/event4 error. ";
		std::cerr << "errno = " << errno << " : " << strerror(errno) << std::endl;
		exit(errno);
	}
	std::cout << "fd = " << fdmice << std::endl;

	// parsing
	struct input_event ie;
	while(read(fdmice, &ie, sizeof(ie))) {
		std::cout << "type " << std::hex << ie.type;
		std::cout << "\tcode " << std::hex << ie.code;
		std::cout << "\tvalue " << std::hex << ie.value;
		std::cout << std::endl;
//		printf("time %ld.%06ld\ttype %d\tcode %d\tvalue %d\n",
//			ie.time.tv_sec, ie.time.tv_usec,
//			ie.type,
//			ie.code,
//			ie.value);
		if (ie.type == EV_SYN) //0
		{
			// TODO sync
		} // sync event
		else if (ie.type == EV_KEY) // 1 button click/release
		{
			if (ie.code == BTN_LEFT) // 0x110
			{
				if (ie.value == 1)
				{
					std::cout << "left button click" << std::endl;
				}
				else if (ie.value == 0)
				{
					std::cout << "left button release" << std::endl;
				}
			}
			else if (ie.code == BTN_RIGHT) // 0x111
			{
				if (ie.value == 1)
				{
					std::cout << "right button click" << std::endl;
				}
				else if (ie.value == 0)
				{
					std::cout << "right button release" << std::endl;
				}
			}
			else if (ie.code == BTN_MIDDLE) // 0x112
			{
				if (ie.value == 1)
				{
					std::cout << "middle button click" << std::endl;
				}
				else if (ie.value == 0)
				{
					std::cout << "middle button release" << std::endl;
				}
			}
		} // button event
		else if (ie.type == EV_REL) // 2 move
		{
		}
	}

	// close file descriptor
	close(fdmice);

	return 0;
}

