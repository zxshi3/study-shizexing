HEADERS   =  hello.h
SOURCES   =  hello.cpp main.cpp
TARGET    =  hello

# comment
#CONFIG += debug release
#CONFIG += debug_and_release debug_and_release_target
CONFIG += debug_and_release debug_and_release_target

CONFIG(debug, debug|release): DESTDIR = build/debug
CONFIG(release, debug|release): DESTDIR = build/release

OBJECTS_DIR = $$DESTDIR/.obj
