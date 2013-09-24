/**
 * this example is a demo to list all attached usb devices
 */
#include "UsbUtils.h"

#include <iostream>
#include <string>

#include <libusb.h>
#include <stdlib.h>
#include <sys/types.h>

void list_devices(libusb_context * ctx);

int main(int argc, char** argv) {
	int r;
	// initialize context
	r = libusb_init(NULL);
	if (r) {
		std::cerr << "initialize libusb context fail : " << r << std::endl;
		exit(r);
	}
	
	list_devices(NULL);
	
	// destroy context
	libusb_exit(NULL);
	return 0;
}

void list_devices(libusb_context * ctx) {
	int r;
	libusb_device ** devs;
	ssize_t n = libusb_get_device_list(ctx, &devs);
	if (n < 0) {
		std::cerr << std::dec << "get device list fail : " << n << std::endl;
		exit(n);
	}
	
	std::cout << n << " devices\n\n";
	
	for (int i = 0; i < n; i++) {
		std::cout << "device[" << i << "] : \n";
		print_dev("\t", devs[i]);
		std::cout << std::endl;
	}
	
	const int RELEASE_REFERENCE_COUNT = 1;
	libusb_free_device_list(devs, RELEASE_REFERENCE_COUNT);
}


