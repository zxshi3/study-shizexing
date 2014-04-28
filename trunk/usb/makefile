CC=gcc

LSUSB_SRCS = lsusb.cpp \
	UsbUtils.cpp
	
COMMON_INC = -I/usr/include/libusb-1.0

COMMON_LIBS = -lusb-1.0 \
	-lstdc++

AOATEST_SRCS = UsbUtils.cpp \
	AOATest.cpp
	
ASYNC_AOATEST_SRCS = UsbUtils.cpp \
	AOATest2.cpp



all: lsusb aoatest async_aoatest

lsusb: ${LSUSB_SRCS}
	${CC} ${LSUSB_SRCS} ${COMMON_INC} -o lsusb ${COMMON_LIBS}

aoatest: ${AOATEST_SRCS}
	${CC} ${AOATEST_SRCS} ${COMMON_INC} -o aoatest ${COMMON_LIBS}
	
async_aoatest: ${ASYNC_AOATEST_SRCS}
	${CC} ${ASYNC_AOATEST_SRCS} ${COMMON_INC} -o async_aoatest ${COMMON_LIBS}

clean:
	rm *.o
