#include "UsbUtils.h"
#include <iostream>

void print_device_descriptor(const std::string & prefix, libusb_device_descriptor & desc, libusb_device_handle * handle);
void print_config_descriptor(const std::string & prefix, libusb_config_descriptor & config, libusb_device_handle * handle);
std::string speed2str(int speed);

void print_dev(const std::string & prefix, libusb_device * dev) {
	int r;
	char buf[4096];

	libusb_device_handle * handle = NULL;
	r = libusb_open(dev, &handle);
	if (r != LIBUSB_SUCCESS) { // libusb_error
		std::cerr << prefix << "open ret : " << r << ". handle : 0x" << handle << std::endl;
	}

	libusb_device_descriptor desc;
	r = libusb_get_device_descriptor(dev, &desc);
	std::cout << prefix << "device descriptor :\n";
	print_device_descriptor(prefix + "\t", desc, handle);
	
	libusb_config_descriptor* config;

	r = libusb_get_active_config_descriptor(dev, &config);
	std::cout << prefix << "active config :\n";
	print_config_descriptor(prefix + "\t", *config, handle);
	libusb_free_config_descriptor(config);
	
	for (int i = 0; i < desc.bNumConfigurations; i++) {
		r = libusb_get_config_descriptor(dev, i, &config);
		std::cout << prefix << "config[" << i << "] :\n";
		print_config_descriptor(prefix + "\t", *config, handle);
		libusb_free_config_descriptor(config);
	}
	
	uint8_t bus = libusb_get_bus_number(dev);
	uint8_t addr = libusb_get_device_address(dev);
	int speed = libusb_get_device_speed(dev);
	// int max_packet_size = libusb_get_max_packet_size(dev, unsigned char endpoint);
	// libusb_get_max_iso_packet_size(dev, unsigned char endpoint);
	std::cout << prefix << "bus : " << (int)bus << std::endl;
	std::cout << prefix << "addr : " << (int)addr << std::endl;
	std::cout << prefix << "speed : " << speed2str(speed) << std::endl;
	
	libusb_close(handle);
}

void print_device_descriptor(const std::string & prefix, libusb_device_descriptor & desc, libusb_device_handle * handle) {
	int r;
	char buf[4096];
	std::cout << prefix << "descriptor length : " << (int)desc.bLength << std::endl;
	std::cout << prefix << "descriptor type : " << (int)desc.bDescriptorType << std::endl;	// LIBUSB_DT_DEVICE
	std::cout << prefix << "usb spec : 0x" << std::hex << desc.bcdUSB << std::dec << std::endl;
	std::cout << prefix << "class : " << (int)desc.bDeviceClass << std::endl;		// libusb_class_code
	std::cout << prefix << "subclass : " << (int)desc.bDeviceSubClass << std::endl;
	std::cout << prefix << "protocol : " << (int)desc.bDeviceProtocol << std::endl;
	std::cout << prefix << "max packet size for endpoint 0 : " << (int)desc.bMaxPacketSize0 << std::endl;
	if (desc.idVendor == 0x18D1) {
		std::cout << prefix << "VID [Google's ID] : 0x" << std::hex << (int)desc.idVendor << std::endl;
	} else {
		std::cout << prefix << "VID : 0x" << std::hex << (int)desc.idVendor << std::endl;
	}
	if (desc.idVendor == 0x18D1 && (desc.idProduct == 0x2D00 || desc.idProduct == 0x2D01)) {
		if (desc.idProduct == 0x2D00) {
			std::cout << prefix << "PID [AOAP] : 0x" << (int)desc.idProduct << std::dec << std::endl;
		} else {	// 0x2D01
			std::cout << prefix << "PID [AOAP + ADB] : 0x" << (int)desc.idProduct << std::dec << std::endl;
		}
	} else {
		std::cout << prefix << "PID : 0x" << (int)desc.idProduct << std::dec << std::endl;
	}
	std::cout << prefix << "release number : " << (int)desc.bcdDevice << std::endl;

	std::cout << prefix << "manufacturer string index: " << (int)desc.iManufacturer << std::endl;
	if (handle) {
		r = libusb_get_string_descriptor_ascii(handle, desc.iManufacturer, (unsigned char*)buf, sizeof(buf));
		std::string strManufacturer(&buf[0]);
		std::cout << prefix << "manufacturer : " << strManufacturer << std::endl;
	}

	std::cout << prefix << "product string index : " << (int)desc.iProduct << std::endl;
	if (handle) {
		r = libusb_get_string_descriptor_ascii(handle, desc.iProduct, (unsigned char*)buf, sizeof(buf));
		std::string strProduct(&buf[0]);
		std::cout << prefix << "product : " << strProduct << std::endl;
	}

	std::cout << prefix << "serial number string index : " << (int)desc.iSerialNumber << std::endl;
	if (handle) {
		r = libusb_get_string_descriptor_ascii(handle, desc.iSerialNumber, (unsigned char*)buf, sizeof(buf));
		std::string strSerialNumber(&buf[0]);
		std::cout << prefix << "S/N : " << strSerialNumber << std::endl;
	}

	std::cout << prefix << "numbre of configurations : " << (int)desc.bNumConfigurations << std::endl;
}

void print_interface(const std::string & prefix, const libusb_interface & interface, libusb_device_handle * handle);
void print_config_descriptor(const std::string & prefix, libusb_config_descriptor & desc, libusb_device_handle * handle) {
	int r;
	char buf[4096];
	std::cout << prefix << "descriptor length : " << (int)desc.bLength << std::endl;
	std::cout << prefix << "descriptor type : " << (int)desc.bDescriptorType << std::endl;	// LIBUSB_DT_CONFIG
	std::cout << prefix << "total length : " << (int)desc.wTotalLength << std::endl;
	std::cout << prefix << "number of interface : " << (int)desc.bNumInterfaces << std::endl;
	std::cout << prefix << "config id : " << (int)desc.bConfigurationValue << std::endl;

	std::cout << prefix << "config string index : " << (int)desc.iConfiguration << std::endl;
	if (handle) {
		r = libusb_get_string_descriptor_ascii(handle, desc.iConfiguration, (unsigned char*)buf, sizeof(buf));
		std::cout << prefix << "config : " << std::string(buf) << std::endl;
	}
	
	std::cout << prefix << "config attributes : 0x" << std::hex << (int)desc.bmAttributes << std::dec << std::endl;
	std::cout << prefix << "max power consumption : " << (2 * desc.MaxPower) << std::endl;
	for (int i = 0; i < desc.bNumInterfaces; i++) {
		std::cout << prefix << "interface [" << i << "]\n";
		print_interface(prefix + "\t", desc.interface[i], handle);
	}
	std::cout << prefix << "extra descriptor (" << desc.extra_length << "): " << std::string((const char*)desc.extra, desc.extra_length) << std::endl;
}

std::string speed2str(int speed) {
	switch(speed) {
	case LIBUSB_SPEED_SUPER:
		return std::string("super speed (5000MBit/s)");
	case LIBUSB_SPEED_HIGH:
		return std::string("high speed (480MBit/s)");
	case LIBUSB_SPEED_FULL:
		return std::string("full speed (12MBit/s)");
	case LIBUSB_SPEED_LOW:
		return std::string("low speed (1.5MBit/s)");
	case LIBUSB_SPEED_UNKNOWN:
		return std::string("unknow speed");
	default:
		return std::string("invalid speed");
	}
}

void print_interface_descriptor(const std::string & prefix, const libusb_interface_descriptor & desc, libusb_device_handle * handle);
void print_interface(const std::string & prefix, const libusb_interface & interface, libusb_device_handle * handle) {
	std::cout << prefix << "number of interface descriptors (aka alternate settings) : " << interface.num_altsetting << std::endl;
	for (int i = 0; i < interface.num_altsetting; i++) {
		std::cout << prefix << "interface descriptor [" << i << "]\n";
		print_interface_descriptor(prefix + "\t", interface.altsetting[i], handle);
	}
}

void print_endpoint_descriptor(const std::string & prefix, const libusb_endpoint_descriptor & desc, libusb_device_handle * handle);
void print_interface_descriptor(const std::string & prefix, const libusb_interface_descriptor & desc, libusb_device_handle * handle) {
	int r;
	char buf[4096];
	std::cout << prefix << "descriptor length : " << (int)desc.bLength << std::endl;
	std::cout << prefix << "descriptor type : " << (int)desc.bDescriptorType << std::endl;	// LIBUSB_DT_INTERFACE
	std::cout << prefix << "interface No. : " << (int)desc.bInterfaceNumber << std::endl;
	std::cout << prefix << "descriptor id (aka id used to select this alternate setting) : " << (int)desc.bAlternateSetting << std::endl;
	std::cout << prefix << "number of endpoint : " << (int)desc.bNumEndpoints << std::endl;
	std::cout << prefix << "interface class : " << (int)desc.bInterfaceClass << std::endl;	//\ref libusb_class_code
	std::cout << prefix << "interface subclass : " << (int)desc.bInterfaceSubClass << std::endl;
	std::cout << prefix << "interface protocol : " << (int)desc.bInterfaceProtocol << std::endl;

	std::cout << prefix << "interface string descriptor index : " << (int)desc.iInterface << std::endl;
	if (handle) {
		r = libusb_get_string_descriptor_ascii(handle, desc.iInterface, (unsigned char*)buf, sizeof(buf));
		std::cout << prefix << "interface : " << std::string(buf) << std::endl;
	}

	for (int i = 0; i < desc.bNumEndpoints; i++) {
		std::cout << prefix << "endpoint[" << i << "] :\n";
		print_endpoint_descriptor(prefix + "\t", desc.endpoint[i], handle);
	}

	// std::cout << prefix << "extra descriptor (" << desc.extra_length << "): " << std::string((const char*)desc.extra, desc.extra_length) << std::endl;
}

void print_endpoint_descriptor(const std::string & prefix, const libusb_endpoint_descriptor & desc, libusb_device_handle * handle) {
	int r;
	char buf[4096];
	std::cout << prefix << "descriptor length : " << (int)desc.bLength << std::endl;
	std::cout << prefix << "descriptor type : " << (int)desc.bDescriptorType << std::endl;	// LIBUSB_DT_ENDPOINT
	/** The address of the endpoint described by this descriptor. Bits 0:3 are
	 * the endpoint number. Bits 4:6 are reserved. Bit 7 indicates direction,
	 * see \ref libusb_endpoint_direction.
	 */
	// uint8_t  bEndpointAddress;
	bool in = desc.bEndpointAddress & 0x80;
	if (in) {
		std::cout << prefix << "endpoint address [IN]: 0x" << std::hex << (int)desc.bEndpointAddress << std::endl;
	} else {
		std::cout << prefix << "endpoint address [OUT]: 0x" << std::hex << (int)desc.bEndpointAddress << std::endl;
	}

	/** Attributes which apply to the endpoint when it is configured using
	 * the bConfigurationValue. Bits 0:1 determine the transfer type and
	 * correspond to \ref libusb_transfer_type. Bits 2:3 are only used for
	 * isochronous endpoints and correspond to \ref libusb_iso_sync_type.
	 * Bits 4:5 are also only used for isochronous endpoints and correspond to
	 * \ref libusb_iso_usage_type. Bits 6:7 are reserved.
	 */
	// uint8_t  bmAttributes;
	std::string type;
	switch(desc.bmAttributes & 0x3) {
	case 0x0:
		type = "control";
		break;
	case 0x1:
		type = "isochronous";
		break;
	case 0x2:
		type = "bulk";
		break;
	case 0x3:
		type = "interrupt";
		break;
	}
	std::cout << prefix << "endpoint attributes [" << type << "] : 0x" << (int)desc.bmAttributes << std::dec << std::endl;

	/** Maximum packet size this endpoint is capable of sending/receiving. */
	// uint16_t wMaxPacketSize;
	std::cout << prefix << "max packet size : " << (int)desc.wMaxPacketSize << std::endl;

	/** Interval for polling endpoint for data transfers. */
	// uint8_t  bInterval;
	std::cout << prefix << "polling interval : " << (int)desc.bInterval << std::endl;

	/** For audio devices only: the rate at which synchronization feedback
	 * is provided. */
	// uint8_t  bRefresh;
	std::cout << prefix << "refresh (audio device only) : " << (int)desc.bRefresh << std::endl;

	/** For audio devices only: the address if the synch endpoint */
	// uint8_t  bSynchAddress;
	std::cout << prefix << "sync address (audio device only) : " << (int)desc.bSynchAddress << std::endl;

	/** Extra descriptors. If libusb encounters unknown endpoint descriptors,
	 * it will store them here, should you wish to parse them. */
	// const unsigned char *extra;

	/** Length of the extra descriptors, in bytes. */
	// int extra_length;
}

