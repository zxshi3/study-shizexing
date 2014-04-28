/**
 * this is a test to active accessory communication mode on phone
 * use asychronized io
 */
#include "UsbUtils.h"
#include <iostream>
#include <errno.h>
#include <libusb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>	//strsignal

/** \ingroup syncio
 * Perform a USB control transfer.
 *
 * The direction of the transfer is inferred from the bmRequestType field of
 * the setup packet.
 *
 * The wValue, wIndex and wLength fields values should be given in host-endian
 * byte order.
 *
 * \param dev_handle a handle for the device to communicate with
 * \param bmRequestType the request type field for the setup packet
 * \param bRequest the request field for the setup packet
 * \param wValue the value field for the setup packet
 * \param wIndex the index field for the setup packet
 * \param data a suitably-sized data buffer for either input or output
 * (depending on direction bits within bmRequestType)
 * \param wLength the length field for the setup packet. The data buffer should
 * be at least this size.
 * \param timeout timeout (in millseconds) that this function should wait
 * before giving up due to no response being received. For an unlimited
 * timeout, use value 0.
 * \returns on success, the number of bytes actually transferred
 * \returns LIBUSB_ERROR_TIMEOUT if the transfer timed out
 * \returns LIBUSB_ERROR_PIPE if the control request was not supported by the
 * device
 * \returns LIBUSB_ERROR_NO_DEVICE if the device has been disconnected
 * \returns another LIBUSB_ERROR code on other failures
 */
//int API_EXPORTED libusb_control_transfer(libusb_device_handle *dev_handle,
//	uint8_t bmRequestType, uint8_t bRequest, uint16_t wValue, uint16_t wIndex,
//	unsigned char *data, uint16_t wLength, unsigned int timeout);

/** \ingroup asyncio
 * Setup packet for control transfers. */
//struct libusb_control_setup {
	/** Request type. Bits 0:4 determine recipient, see
	 * \ref libusb_request_recipient. Bits 5:6 determine type, see
	 * \ref libusb_request_type. Bit 7 determines data transfer direction, see
	 * \ref libusb_endpoint_direction.
	 */
	//uint8_t  bmRequestType;

	/** Request. If the type bits of bmRequestType are equal to
	 * \ref libusb_request_type::LIBUSB_REQUEST_TYPE_STANDARD
	 * "LIBUSB_REQUEST_TYPE_STANDARD" then this field refers to
	 * \ref libusb_standard_request. For other cases, use of this field is
	 * application-specific. */
	//uint8_t  bRequest;

	/** Value. Varies according to request */
	//uint16_t wValue;

	/** Index. Varies according to request, typically used to pass an index
	 * or offset */
	//uint16_t wIndex;

	/** Number of bytes to transfer */
	//uint16_t wLength;
//};

/*
 * return : true for success, vice versa
 */
bool GetProtocol(libusb_device_handle * handle);
bool SendAccessoryInfos(libusb_device_handle * handle);
bool SendResetCommand(libusb_device_handle * handle);
void list_devices(libusb_context * ctx);
bool g_quit = false;

/**
 * Get active interface number and input/output endpoint
 * @Param handle [in] usb device handle
 * @Param dev [in] usb device
 * @Param interface [out] interface number
 * @Param endpoint_in [out] input endpoint address
 * @Param endpoint_out [out] output endpoint address
 * @return :
 *	true for success, 
 *	false for failure
 */
bool GetCommunicationEndpoint(libusb_device_handle * handle, libusb_device * dev, int & interface, unsigned char & endpoint_in, unsigned char & endpoint_out);

void sig_handler(int signum) {
	printf("handle : %s\n", strsignal(signum));
	g_quit = true;
}

void usbSendComplete(struct libusb_transfer * transfer);
void usbRecvComplete(struct libusb_transfer * transfer);

char readBuf[1024] = {0};		// if this cause crash issue, declare it as global variable

int main(int argc, char** argv) {
	libusb_device_handle * handle = NULL;
	libusb_context * ctx = NULL;
	uint16_t vid = 0x18d1;
	uint16_t pid = 0x4e22;
	int r;
	
	if (SIG_ERR == signal(SIGINT, &sig_handler)) {
		std::cerr << "can not catch SIGINT\n";
	}
	signal(SIGTERM, &sig_handler);
	signal(SIGKILL, &sig_handler);
	
	r = libusb_init(&ctx);
	if (r != LIBUSB_SUCCESS) {
		std::cerr << "libusb init fail. " << libusb_error_name(r) << std::endl;
		exit(r);
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	// Switch to aoap mode
	////////////////////////////////////////////////////////////////////////////////////////////////
	handle = libusb_open_device_with_vid_pid(ctx, vid, pid);
	if (handle == NULL) {
		std::cerr << "open device " << vid << ":" << pid << " error. " << libusb_error_name(errno) << std::endl;
		// exit(errno);
	} else {
		libusb_device * dev = NULL;
		dev = libusb_get_device(handle);
		dev = libusb_ref_device(dev);
	
		print_dev("", dev);
	
		{
			// 1. Get AOA Protocol on phone
			if (GetProtocol(handle) == false) {
				std::cerr << "Get AOA Protocol error\n";
				libusb_unref_device(dev);
				libusb_exit(ctx);
				exit(-1);
			}
			// 2. send accessory information
			if (SendAccessoryInfos(handle) == false) {
				std::cerr << "Send Accessory infos error\n";
				libusb_unref_device(dev);
				libusb_exit(ctx);
				exit(-2);
			}
			// 3. send reset command
			if (SendResetCommand(handle) == false) {
				std::cerr << "Send Reset command error\n";
				libusb_unref_device(dev);
				libusb_exit(ctx);
				exit(-3);
			}
		}
		
		std::cout << "phone reset to aoa mode\n";
	
		libusb_unref_device(dev);
	}
	std::cout << std::endl;
	std::cout << "===========================================================================\n";
	// list_devices(ctx);
	
	std::cout << "===========================================================================\n";
	// open aoa phone
	pid = 0x2D01;
	handle = NULL;
	int cnt = 60;
	while (handle == NULL) {
		handle = libusb_open_device_with_vid_pid(ctx, vid, pid);
		if (handle != NULL) {
			break;
		}
		sleep(1);
		if (--cnt < 0) {
			break;
		}
	}
	if (handle == NULL) {
		std::cerr << "open device " << vid << ":" << pid << " error. " << libusb_error_name(errno) << std::endl;
		// exit(errno);
	} else {
		for (int i = 0; i < 15; i++) {
			sleep(1);
			std::cout << "." << std::flush;
		}
		std::cout << std::endl;
		std::cout << "open device " << vid << ":" << pid << " success." << std::endl;
		libusb_device * dev = NULL;
		dev = libusb_get_device(handle);
		dev = libusb_ref_device(dev);
	
		print_dev("", dev);
		
		std::cout << std::endl;
		int interface = 0;
		unsigned char endpoint_in = 0, endpoint_out = 0;
		GetCommunicationEndpoint(handle, dev, interface, endpoint_in, endpoint_out);
		std::cout << std::endl;
		std::cout << "Get interface : " << (int)interface << std::endl;
		std::cout << "Get input endpoint address : 0x" << std::hex << (int)endpoint_in << std::endl;
		std::cout << "Get output endpoint address : 0x" << (int)endpoint_out << std::dec << std::endl;
	
		// 1. check whether kernel use this device or not
		int active = libusb_kernel_driver_active(handle, interface);
		std::cout << "interface [" << interface << "] active = " << active << std::endl;
		if (active == 1) {
			r = libusb_detach_kernel_driver(handle, interface);
			if (r != LIBUSB_SUCCESS) {
				std::cerr << "detach kernel drive error. " << libusb_error_name(r) << std::endl;
			}
		} else if (active != 0) {// 0 means no kernel drive active 
			std::cerr << "query kernel drive active fail. " << libusb_error_name(active) << std::endl;
		} 
		
		// 2. claim interface
		r = libusb_claim_interface(handle, interface);
		if (r != LIBUSB_SUCCESS) {
			std::cerr << "claim interface error. " << libusb_error_name(r) << std::endl;
		} else {
			std::cout << "claim interface[" << interface << "] success\n";
		}
		
		{
		// TODO
		// communication
		// int LIBUSB_CALL libusb_bulk_transfer(libusb_device_handle *dev_handle,
		//		unsigned char endpoint, unsigned char *data, int length,
		//		int *actual_length, unsigned int timeout);
		char buf[1024];
		memset(buf, 0, sizeof(buf));
		const char * info = "USB accessory [Ubuntu 12.04 host]";
		//const char * info = "Telenav USB accessory";
		//const char * info = "brad.shi";
		strncpy(buf, info, sizeof(buf));
		int actual_transfer_bytes = 0;
		int timeout = 1000;
		
		r = libusb_bulk_transfer(handle, endpoint_out, (unsigned char*)&buf[0], strlen(buf) + 1, &actual_transfer_bytes, timeout);
		if (r != LIBUSB_SUCCESS) {
			std::cerr << "bulk transfer error [hu -> phone]. " << libusb_error_name(r) << std::endl;
		} else {
			std::cout << "WOW, bulk transfer success [hu -> phone]. " << actual_transfer_bytes << " bytes transfered." << std::endl;
		}
				
		memset(buf, 0, sizeof(buf));
		timeout = 5 * 1000;
		r = libusb_bulk_transfer(handle, endpoint_in, (unsigned char*)&buf[0], sizeof(buf), &actual_transfer_bytes, timeout);
		if (r != LIBUSB_SUCCESS) {
			std::cerr << "bulk transfer error [phone -> hu]. " << libusb_error_name(r) << std::endl;
		} else {
			std::cout << "WOW, bulk transfer success [phone -> hu]. " << actual_transfer_bytes << " bytes transfered." << std::endl;
			std::cout << "RECEIVE : " << buf << std::endl;
		}
		
		} // end communication
		
		{ // async io
			const char * msg = "test usb async io.";
			//char buf[1024];
			char * buf;
			int timeout = 2000;
			buf = (char*)malloc(strlen(msg) + 1);
			memset(buf, 0, strlen(msg) + 1);
			strncpy(buf, msg, strlen(msg));
			std::cout << "buf : " << buf << std::endl;
			std::cout << "strlen(buf) = " << strlen(buf) << std::endl;
			// char readBuf[1024] = {0};		// if this cause crash issue, declare it as global variable
			// struct libusb_transfer inTransfer = {0};	// if this cause crash issue, use , answer : this will cause unexpect result
														// struct libusb_transfer * inTransfer = libusb_alloc_transfer(0);
			// struct libusb_transfer outTransfer = {0};
			struct libusb_transfer * pOutTransfer = libusb_alloc_transfer(0);	// should free it by calling libusb_free_transfer(libusb_transfer *)
			struct libusb_transfer & outTransfer = *pOutTransfer;
			std::cout << "fill out transfer" << std::endl;
			libusb_fill_bulk_transfer(&outTransfer	// struct libusb_transfer*
				, handle					// libusb_device_handle *
				, endpoint_out				// address of the endpoint where this transfer will be sent
				, (unsigned char*)&buf[0]	// data buffer
				, strlen(buf) + 1			// length of data buffer
				, &usbSendComplete			// callback fn, libusb_transfer_cb_fn
				, NULL						// user data to pass to callback function
				, timeout);					// timeout for the transfer in milliseconds
			std::cout << "submit out transfer" << std::endl;
			r = libusb_submit_transfer(&outTransfer);
			if (r != LIBUSB_SUCCESS) {
				std::cout << "r = " << r << ". " << libusb_error_name(r) << std::endl;
			}
			std::cout << "fill in transfer" << std::endl;
			
			struct libusb_transfer * pInTransfer = libusb_alloc_transfer(0);
			struct libusb_transfer & inTransfer = *pInTransfer;
			libusb_fill_bulk_transfer(&inTransfer	// struct libusb_transfer *
				, handle
				, endpoint_in
				, (unsigned char*)&readBuf[0]
				, sizeof(readBuf) - 1
				, &usbRecvComplete			// callback fn, libusb_transfer_cb_fn
				, NULL
				, timeout);
			std::cout << "submit in transfer" << std::endl;
			r = libusb_submit_transfer(&inTransfer);
			if (r != LIBUSB_SUCCESS) {
				std::cout << "r = " << r << ". " << libusb_error_name(r) << std::endl;
			}
		
			for (int i = 0; i < 10; i++) {
				struct timeval tv;
				tv.tv_sec  = 1;     // seconds
				tv.tv_usec = 0;		// milliseconds  ( .1 sec)
				r = libusb_handle_events_timeout(ctx, &tv); 
				sleep(1);
				std::cout << '.';
			}
			std::cout << std::endl;
			
			// release libusb_transfer
			std::cout << "LIBUSB_ERROR_NOT_FOUND if the transfer is already complete or cancelled." << std::endl;
			if (pOutTransfer) {
				r = libusb_cancel_transfer(pOutTransfer);
				if (LIBUSB_SUCCESS == r){
					std::cout << "pOutTransfer successfully cancelled\n";
				} else {
					std::cerr << "can out transfer error : " << libusb_error_name(r) << std::endl;
				}
				libusb_free_transfer(pOutTransfer);
				pOutTransfer = NULL;
			}
			if (pInTransfer) {
				r = libusb_cancel_transfer(pInTransfer);
				if (LIBUSB_SUCCESS == r){
					std::cout << "pInTransfer successfully cancelled\n";
				} else {
					std::cerr << "can in transfer error : " << libusb_error_name(r) << std::endl;
				}
				libusb_free_transfer(pInTransfer);
				pInTransfer = NULL;
			}
			free(buf);
		}
		
		std::cout << "releasing interface..." << std::endl;
		// -2. release interface
		r = libusb_release_interface(handle, interface);
		if (r != LIBUSB_SUCCESS) {
			std::cerr << "release interface error. " << libusb_error_name(r) << std::endl;
		} else {
			std::cout << "release interface[" << interface << "] success\n";
		}
		
		// ??? reset aoa ???
		// SendResetCommand(handle);
		// libusb_reset_device(handle);
		GetProtocol(handle);
		
		// -1. reactive kernel driver?
		if (active == 1) {
			r = libusb_attach_kernel_driver(handle, interface);
			if (r != LIBUSB_SUCCESS) {
				std::cerr << "attach kenel drive error. " << libusb_error_name(r) << std::endl;
			}
		}
			
		libusb_unref_device(dev);
	}
	
	libusb_exit(ctx);
	std::cout << "===========================================================================\n" << std::flush;
	cnt = 5;
	while(g_quit == false) {
		if (cnt < 0) {
			std::cout << "extends 60s. quit*\n";
			break;
		}
		sleep(1);
		cnt--;
	}
	
	
	return 0;
}

bool GetProtocol(libusb_device_handle * handle) {
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
	return success;
}

enum EAccessoryInfoStringIndex {
	E_ASID_manufacturer_name 	= 0,
	E_ASID_model_name 			= 1,
	E_ASID_description			= 2,
	E_ASID_version				= 3,
	E_ASID_URI					= 4,
	E_ASID_serial_number		= 5
};

bool SendAccessoryInfo(libusb_device_handle * handle, uint16_t wIndex, const std::string & desc) {
	bool success = true;
	struct libusb_control_setup cp = {0};
	// ref http://source.android.com/accessories/aoa.html
	cp.bmRequestType = LIBUSB_RECIPIENT_DEVICE /* 0 */ | LIBUSB_REQUEST_TYPE_VENDOR /* 0x02 << 5 */ | LIBUSB_ENDPOINT_OUT /* 0x0 */;
	cp.bRequest = 52;
	cp.wValue = 0;
	cp.wIndex = wIndex;
	cp.wLength = desc.length() + 1;
	unsigned int timeout = 2000;	// in milliseconds
	unsigned char buf[255];
	for (int i = 0; i < cp.wLength; i++) {
		buf[i] = (unsigned char)desc[i];
	}
	int n = libusb_control_transfer(handle, cp.bmRequestType, cp.bRequest, cp.wValue, cp.wIndex, buf, cp.wLength, timeout);
	if (n < 0) {
		std::cerr << "Send Accessory Info [" << desc << "] error : " << libusb_error_name(n) << std::endl;
		success = false;
	} 
	return success;
}

bool SendAccessoryInfos(libusb_device_handle * handle) {
	bool success = true;
	//std::string strManufacturerName = "Telenav, Inc";
	std::string strModelName = "CarConnect-USB";
	std::string strManufacturerName = "Telenav";
	//std::string strModelName = "CarConnect";
	std::string strDescription = "USB connection";
	std::string strVersion = "2.006";
	std::string strURI = "http://www.telenav.com";
	std::string strSN = "2:006";
	
	std::cout << "version : " << strVersion << ". len = " << strVersion.length() << std::endl;
	SendAccessoryInfo(handle, E_ASID_manufacturer_name, strManufacturerName);
	SendAccessoryInfo(handle, E_ASID_model_name, strModelName);
	SendAccessoryInfo(handle, E_ASID_description, strDescription);
	SendAccessoryInfo(handle, E_ASID_version, strVersion);
	SendAccessoryInfo(handle, E_ASID_URI, strURI);
	SendAccessoryInfo(handle, E_ASID_serial_number, strSN);
	
	return true;
}

bool SendResetCommand(libusb_device_handle * handle) {
	bool success = true;
	struct libusb_control_setup cp = {0};
	// ref http://source.android.com/accessories/aoa.html
	cp.bmRequestType = LIBUSB_RECIPIENT_DEVICE /* 0 */ | LIBUSB_REQUEST_TYPE_VENDOR /* 0x02 << 5 */ | LIBUSB_ENDPOINT_OUT /* 0x0 */;
	cp.bRequest = 53;
	cp.wValue = 0;
	cp.wIndex = 0;
	cp.wLength = 0;
	unsigned int timeout = 1000;	// in milliseconds
	int n = libusb_control_transfer(handle, cp.bmRequestType, cp.bRequest, cp.wValue, cp.wIndex, NULL, cp.wLength, timeout);
	if (n < 0) {
		std::cerr << "Send Reset error : " << libusb_error_name(n) << std::endl;
		success = false;
	} 
	return success;
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

bool GetCommunicationEndpoint(libusb_device_handle * handle, libusb_device * dev, int & interface, unsigned char & endpoint_in, unsigned char & endpoint_out) {
	int r;
	libusb_config_descriptor * config;
	r = libusb_get_active_config_descriptor(dev, &config);
	if (r != LIBUSB_SUCCESS) {
		std::cerr << "Get active config descriptor failure. error : " << libusb_error_name(r) << std::endl;
		return false;
	}
	
	std::cout << "active configuration\n";
	std::string prefix = "\t";
	std::cout << prefix << "number of interface : " << (int)config->bNumInterfaces << std::endl;
	// assert(config.bNumInterfaces == 1);
	// always use config.interface[0]
	const libusb_interface & active_interface = config->interface[0];
	//const libusb_interface & active_interface = config->interface[1];
	
	
	std::cout << prefix << "number of interface descriptors (aka alternate settings) : " << active_interface.num_altsetting << std::endl;
	// always use active_interface.altsetting[0]
	const libusb_interface_descriptor & active_interface_desc = active_interface.altsetting[0];
	interface = active_interface_desc.bInterfaceNumber;
	std::cout << prefix << "interface No. : " << (int)active_interface_desc.bInterfaceNumber << std::endl;
	std::cout << prefix << "number of endpoint : " << (int)active_interface_desc.bNumEndpoints << std::endl;
	for (int i = 0; i < active_interface_desc.bNumEndpoints; i++) {
		std::cout << prefix << "endpoint[" << i << "] :\n";
		const libusb_endpoint_descriptor & endpoint_desc = active_interface_desc.endpoint[i];
		bool in = endpoint_desc.bEndpointAddress & 0x80;
		if (in) {
			std::cout << prefix << "endpoint address [IN]: 0x" << std::hex << (int)endpoint_desc.bEndpointAddress << std::endl;
			endpoint_in = endpoint_desc.bEndpointAddress;
		} else {
			std::cout << prefix << "endpoint address [OUT]: 0x" << std::hex << (int)endpoint_desc.bEndpointAddress << std::endl;
			endpoint_out = endpoint_desc.bEndpointAddress;
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
		switch(endpoint_desc.bmAttributes & 0x3) {
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
		std::cout << prefix << "endpoint attributes [" << type << "] : 0x" << (int)endpoint_desc.bmAttributes << std::dec << std::endl;

		/** Maximum packet size this endpoint is capable of sending/receiving. */
		// uint16_t wMaxPacketSize;
		std::cout << prefix << "max packet size : " << (int)endpoint_desc.wMaxPacketSize << std::endl;
	}
	
	libusb_free_config_descriptor(config);
	return true;
}

std::ostream & operator << (std::ostream & os, enum libusb_transfer_status & status) {
	os << "libusb_transfer_status : ";
	switch(status) {
	case LIBUSB_TRANSFER_COMPLETED:
		os << "Transfer completed without error.\n"
			<< "Note that this does not indicate that the entire amount of requested data was transferred.";
		break;
	case LIBUSB_TRANSFER_ERROR:
		os << "Transfer failed.";
		break;
	case LIBUSB_TRANSFER_TIMED_OUT:
		os << "Transfer timed out.";
		break;
	case LIBUSB_TRANSFER_CANCELLED:
		os << "Transfer was cancelled.";
		break;
	case LIBUSB_TRANSFER_STALL:
		os << "For bulk/interrupt endpoints: halt condition detected (endpoint stalled).\n"
			<< "For control endpoints: control request not supported.";
		break;
	case LIBUSB_TRANSFER_NO_DEVICE:
		os << "Device was disconnected.";
		break;
	case LIBUSB_TRANSFER_OVERFLOW:
		os << "Device sent more data than requested.";
		break;
	}
	os << "\n";
}

void usbSendComplete(struct libusb_transfer * transfer) {
	std::cout << "in usbSendComplete" << std::endl;
	if (transfer) {
		std::cout << transfer->status;
		std::cout << "actual transfer " << transfer->actual_length << " bytes" << std::endl;
	} else {
		std::cerr << "transfer is null. in usbSendComplete" << std::endl;
	}
}

void usbRecvComplete(struct libusb_transfer * transfer) {
	std::cout << "in usbRecvComplete" << std::endl;
	if (transfer) {
		std::cout << transfer->status;
		std::cout << "actual transfer " << transfer->actual_length << " bytes" << std::endl;
		std::cout << "RECEIVE : " << (char*)transfer->buffer << std::endl;
	} else {
		std::cerr << "transfer is null. in usbRecvComplete" << std::endl;
	}
}


