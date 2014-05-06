PREFIX = llvm-
CC = $(PREFIX)gcc
CXX = $(PREFIX)g++

CFLAGS = -I.
LDLIBS = -lm

JSON_SRCS = \
	cJSON.c

SRCS = \
	main.cpp

OBJS = $(JSON_SRCS:.c=.o) $(SRCS:.cpp=.o)

all : bin

obj : $(SRCS) $(JSON_SRCS)
	$(CXX) -c $(SRCS) $(CFLAGS)
	
bin : obj $(OBJS)
	$(CXX) -o test $(OBJS) $(LDLIBS)

clean:
	rm $(OBJS) test
