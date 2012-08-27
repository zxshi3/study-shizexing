CC = g++

BOOST_HOME = ./boost-trunk

CFLAGS = -I${BOOST_HOME}

LDFLAGS = 

LDLIBS = 

all : die

die: test.cpp
	${CC} -o ./bin/test01 test.cpp ${CFLAGS} ${LDFLAGS} ${LDLIBS}

clean:
	rm -rf ./bin/* 
