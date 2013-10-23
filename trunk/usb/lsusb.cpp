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

void checkAOACompatible(libusb_device * dev);
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
		checkAOACompatible(devs[i]);
	}
	
	const int RELEASE_REFERENCE_COUNT = 1;
	libusb_free_device_list(devs, RELEASE_REFERENCE_COUNT);
}


void checkAOACompatible(libusb_device * dev) {
	libusb_device_handle * handle = NULL;
	int r = libusb_open(dev, &handle);
	if (r != LIBUSB_SUCCESS) {
		std::cout << "open device handle error. " << libusb_error_name(r) << "\n";
		return;
	}
	
	bool success = false;
	// \ref struct libusb_control_setup
	struct libusb_control_setup cp = {0};
	// ref http://source.android.com/accessories/aoa.html
	cp.bmRequestType = LIBUSB_RECIPIENT_DEVICE /* 0 */ | LIBUSB_REQUEST_TYPE_VENDOR /* 0x02 << 5 */ | LIBUSB_ENDPOINT_IN /* 0x80 */;
	cp.bRequest = 51;
	cp.wValue = 0;
	cp.wIndex = 0;
	unsigned char buf[255];	/* some devices choke on size > 255 */
	cp.wLength = sizeof(buf);
	unsigned int timeout = 1000;	// in milliseconds
	int n = libusb_control_transfer(handle, cp.bmRequestType, cp.bRequest, cp.wValue, cp.wIndex, &buf[0], cp.wLength, timeout);
	if (n > 0) {
		std::cout << "read " << n << " bytes from phone\n";
		std::cout << "Protocol : 0x";
		for (int i = 0; i < n; i++) {
			std::cout << (int)buf[i] << " ";
		}
		std::cout << std::endl;
		uint16_t leProtocol = buf[0] + buf[1] << 8;
		uint16_t nProtocol = libusb_le16_to_cpu(leProtocol);
		std::cout << "Protocol : 0x" << (int)nProtocol << std::endl;
		success = true;
	} else if (n == 0) {
		std::cout << "read nothing\n";
	} else {
		std::cerr << "Get Protocol from phone error : " << libusb_error_name(n) << std::endl;
	}
	return;
}

