CC = gcc

CFLAGS = -I.
LDLIBS = -lm

SRCS = \
	cJSON.c \
	main.c

OBJS = $(SRCS:.c=.o)

all : bin

obj : $(SRCS)
	$(CC) -c $(SRCS) $(CFLAGS)
	
bin : obj $(OBJS)
	$(CC) -o test $(OBJS) $(LDLIBS)

clean:
	rm $(OBJS) test
