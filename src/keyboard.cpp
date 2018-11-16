#include "keyboard.h"
#include <deque>
#include <string>
#include <stdexcept>
extern "C"
{
#include <linux/input.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <poll.h>
}


struct keyboard {
    int fd;
    int countK_a = 0;
    int countK_w = 0;
    int countK_s = 0;
    int countK_d = 0;
    keyboard() {
        if ((fd = open("/dev/input/event4", O_RDONLY)) <= 0) {
            throw std::runtime_error("Cannot use kb device");
        }
    }
    bool try_read() noexcept {
        int count = 0;
        pollfd pfd = {fd, POLLIN, 0};
        input_event event;
        while(poll(&pfd, 1, 0) > 0)
            if(read(fd, &event, sizeof (event)) == sizeof (event)) {
                switch (event.code) {
                case 17:
                    ++countK_w;
                    break;
                case 30:
                    ++countK_a;
                    break;
                case 31:
                    ++countK_s;
                    break;
                case 32:
                    ++countK_d;
                    break;
                }
                ++count;
            }
        return  count != 0;
    }
};

keyboard kb;
KBEvent getKBchange() noexcept {
    KBEvent a;
    if(!kb.try_read())
        return a;
    a.countK_a = kb.countK_a; kb.countK_a = 0;
    a.countK_w = kb.countK_w; kb.countK_w = 0;
    a.countK_s = kb.countK_s; kb.countK_s = 0;
    a.countK_d = kb.countK_d; kb.countK_d = 0;
    return a;
}

