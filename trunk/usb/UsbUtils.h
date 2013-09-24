#ifndef	__USB_UTILS_H__
#define	__USB_UTILS_H__

#include <string>
#include <libusb.h>

void print_dev(const std::string & prefix, libusb_device * dev);

#endif	//!__USB_UTILS_H__

