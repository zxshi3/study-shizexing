/**
 * epoll is an example application for showing how to use epoll
 *
 * Reference:
 *  https://banu.com/blog/2/how-to-use-epoll-a-complete-example-in-c/
 *  http://linux.die.net/man/7/epoll
 *
 * More Info:
 *  libevent & libev is designed for asynchronized io for multi platform.
 */
#include <iostream>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <time.h>
#include <unistd.h>

#define ZX_INVALID_FD 0
class FileReader {
public:
    FileReader(const std::string & name) 
        : fd(ZX_INVALID_FD)
        , epfd(ZX_INVALID_FD)
        , name(name) {
        
    }
    bool open() {
        fd = open(name.c_str(), O_RDONLY);
        if (ZX_INVALID_FD == fd) {
            std::cerr << "FileReader : open " << name << " fail. " << strerror(errno) << "\n";
            return false;
        }
        int flags = fcntl(fd, F_GETFL, 0);
        flags |= O_NONBLOCK;
        int r = fcntl(fd, F_SETFL, flags);
        if (r == -1) {
            std::cerr << "FileReader : set non block error. " << strerror(errno) << "\n";
            return true;
        }

        epfd = epoll_create1(0);
        if (ZX_INVALID_FD == epfd) {
            std::cerr << "FileReader : create epoll fail. " << strerror(errno) << "\n";
            return true;
        }
        struct epoll_event epev;
        epev.events = EPOLLIN;
        epev.data.fd = fd;
        r = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &epev);
        if (-1 == r) {
            std::cerr << "FileReader : add read event listener to epoll fail. " << strerror(errno) << "\n";
            close(epfd);
            epfd = ZX_INVALID_FD;
        }
        return true;
    }
    bool read(std::string & buffer) {
        if (ZX_INVALID_FD == fd) {
            std::cerr << "FileReader : in reading, invalid file descriptor.\n";
            return false;
        }
        if (ZX_INVALID_FD == epfd) {
            std::cerr << "FileReader : epoll file descriptor is invalid, sleep for a while\n";
            usleep (100 * 1000);
        } else {
            struct epoll_event epev;
            int r = epoll_wait(epfd, &epev, 1, 1000);   // only read event we are interesting
            if (r == -1) {
                std::cerr << "FileReader : epoll wait error. " << strerror(errno) << "\n";
                return false;
            } else if (r == 0) {
                std::cout << "FileReader : no data available\n";
                return true;
            } else {
                assert(epfd.events & EPOLLIN);
                assert(epfd.data.fd == fd);
            }
        }
        ssize_t nr = read(fd, buf, sizeof(buf) - 1);
        if (nr <= 0) {
            std::cout << "FileReader : no data available";
            return true;
        }
        buf[nr] = '\0';
        buffer = std::string(buf);
        return true;
    }
private:
    void close() {
        if (ZX_INVALID_FD != epfd) {
            int r = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
            if (-1 == r) {
                std::cerr << "FileReader : remove read event listener from epoll fail. " << strerror(errno) << "\n";
            }
            r = close(epfd);
            if (-1 == r) {
                std::cerr << "FileReader : close epoll fail. " << strerror(errno) << "\n";
            }
            epfd = ZX_INVALID_FD;
        }
        if (ZX_INVALID_FD != fd) {
            r = close(fd);
            if (-1 == r) {
                std::cerr << "FileReader : close file descriptor fail. " << strerror(errno) << "\n";
            }
            fd = ZX_INVALID_FD;
        }
    }
private:
    int fd;     // ublox file descriptor
    int epfd;   // epoll file descriptor
    char buf[4096];
    std::string name;
};

int main(int argc, char** argv) {
    FileReader reader("/dev/ttyACM0");      // ublox gps
    if (reader.open() == false) {
        std::cerr << "open file error, exit\n";
        exit(errno);
    }
    // here, you can add a new thread to read data. 
    for (int i = 0; i < 25; ++i) {
        std::string msg;
        if (reader.read(msg) == false) {
            std::cerr << "reading error." << std::endl;
        } else {
            std::cout << "data : \n"
                << msg << "\n";
        }
    }
    
    return 0;
}
