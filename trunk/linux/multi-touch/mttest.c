// mttest.c
#include <mtdev.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>

struct TnTouchEvent {
    struct TouchPoint {
        int x;          // x position in global 
        int y;          // y position in global
    };
    
    // TODO zexings, should match with Qt
    enum ETouchEventType {
        E_PRESS = 1,
        E_MOVE = 2,
        E_RELEASE = 3
    };
    
    TouchPoint pt;
    ETouchEventType type;
};

static std::vector<TnTouchEvent> s_touchEvents;
static std::vector<struct input_event> s_rawEvents;

/* year-proof millisecond event time */
typedef __u64 mstime_t;

static int use_event(const struct input_event *ev)
{
#if 0
	return ev->type == EV_ABS && mtdev_is_absmt(ev->code);
#else
	return 1;
#endif
}

// type defined in linux/input.h
// #define EV_SYN			0x00
// #define EV_KEY			0x01        for multi-touch, first touch down / last touch up
// #define EV_ABS			0x03        // code 0 == x, 1 == y 

// code defined in linux/input.h
//// #define BTN_TOUCH		0x14a
// #define ABS_X			0x00
// #define ABS_Y			0x01
// #define ABS_PRESSURE		0x18

/*
 * eg:
 * 00 3 0000 189
 * 00 3 0001 159
 * 00 0 0000 0
 * 00 3 0000 190
 * 00 0 0000 0
 * 00 3 0000 195
 * 00 3 0001 162
 * 00 0 0000 0
 * 00 3 0018 0
 * 00 1 014a 0
 * 00 0 0000 0
*/

static void print_event(const struct input_event *ev)
{
	static const mstime_t ms = 1000;
	static int slot;
	mstime_t evtime = ev->time.tv_usec / ms + ev->time.tv_sec * ms;
	if (ev->type == EV_ABS && ev->code == ABS_MT_SLOT) {
	    fprintf(stderr, "set slot value to %02d\n", ev->value);
		slot = ev->value;
	}
//	fprintf(stderr, "%012llx %02d %01d %04x %d\n",
//		evtime, slot, ev->type, ev->code, ev->value);
   	fprintf(stderr, "%02d %01d %04x %d\n", slot, ev->type, ev->code, ev->value);
}

#define CHECK(dev, name)			\
	if (mtdev_has_mt_event(dev, name))	\
		fprintf(stderr, "   %s\n", #name)

static void show_props(const struct mtdev *dev)
{
	fprintf(stderr, "supported mt events:\n");
	CHECK(dev, ABS_MT_SLOT);
	CHECK(dev, ABS_MT_TOUCH_MAJOR);
	CHECK(dev, ABS_MT_TOUCH_MINOR);
	CHECK(dev, ABS_MT_WIDTH_MAJOR);
	CHECK(dev, ABS_MT_WIDTH_MINOR);
	CHECK(dev, ABS_MT_ORIENTATION);
	CHECK(dev, ABS_MT_POSITION_X);
	CHECK(dev, ABS_MT_POSITION_Y);
	CHECK(dev, ABS_MT_TOOL_TYPE);
	CHECK(dev, ABS_MT_BLOB_ID);
	CHECK(dev, ABS_MT_TRACKING_ID);
	CHECK(dev, ABS_MT_PRESSURE);
	CHECK(dev, ABS_MT_DISTANCE);
}

static void convertRawInputEventToTnTouchEvent(const std::vector<struct input_event> & rawEvents, std::vector<TnTouchEvent> & touchEvents) {
    touchEvents.clear();
    TnTouchEvent e;
    bool xValid = false;
    bool yValid = false;
    bool notMove = false;
    for (std::vector<struct input_event>::const_iterator it = rawEvents.begin(); it != rawEvents.end(); it++) {
        if (it->type == EV_ABS /* 3 */) {
            switch(it->code) {
            case ABS_X /* 0 */:
                xValid = true;
                e.pt.x = it->value;
                break;
            case ABS_Y /* 1 */:
                yValid = true;
                e.pt.y = it->value;
                break;
            case ABS_PRESSURE /* 0x18 */:
                notMove = true;
                if (it->value == 1) {
                    e.type = TnTouchEvent::E_PRESS;
                }
                else if (it->value == 0) {
                    e.type = TnTouchEvent::E_RELEASE;
                }
            }
        }
        if (it->type == EV_SYN) {
            if (notMove == false) {
                e.type = TnTouchEvent::E_MOVE;
            }
            if (xValid && yValid) {
                touchEvents.push_back(e);
            }
            xValid = false;
            yValid = false;
            notMove = false;
        }
    }
}

static void loop_device(int fd)
{
	struct mtdev dev;
	struct input_event ev;
	int ret = mtdev_open(&dev, fd);
	if (ret) {
		fprintf(stderr, "error: could not open device: %d\n", ret);
		return;
	}
	show_props(&dev);
	/* while the device has not been inactive for five seconds */
	while (!mtdev_idle(&dev, fd, 5000)) {
		/* extract all available processed events */
		s_rawEvents.clear();
		s_touchEvents.clear();
		while (mtdev_get(&dev, fd, &ev, 1) > 0) {
			if (use_event(&ev)) {
				print_event(&ev);
		    }
		    s_rawEvents.push_back(ev);
		}
		fprintf(stderr, "------------- loop -----------------\n");
		convertRawInputEventToTnTouchEvent(s_rawEvents, s_touchEvents);
	}
	mtdev_close(&dev);
}

int main(int argc, char *argv[])
{
	int fd;
	if (argc < 2) {
		fprintf(stderr, "Usage: mtdev <device>\n");
		return -1;
	}
	fd = open(argv[1], O_RDONLY | O_NONBLOCK);
	if (fd < 0) {
		fprintf(stderr, "error: could not open device\n");
		return -1;
	}
	if (ioctl(fd, EVIOCGRAB, 1)) {
		fprintf(stderr, "error: could not grab the device\n");
		return -1;
	}
	loop_device(fd);
	ioctl(fd, EVIOCGRAB, 0);
	close(fd);
	return 0;
}
