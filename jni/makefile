CC=g++

CFLAGS=-I${SUN_JAVA_HOME}/include
CFLAGS+=-I${SUN_JAVA_HOME}/include/linux

LDFLAGS=-L${SUN_JAVA_HOME}/jre/lib/i386/server
#LDFLAGS=

LDLIBS=-ljvm
#LDLIBS=

all: exe inv

exe: createJVM.cpp
	${CC} createJVM.cpp -o createJVM ${CFLAGS} ${LDFLAGS} ${LDLIBS}
inv: invoke.cpp
	${CC} invoke.cpp -o invoke ${CFLAGS} ${LDFLAGS} ${LDLIBS}

clean:
	rm -f createJVM *.o invoke

