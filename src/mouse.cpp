#include "mouse.h"
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
class Mouse
{
    int fd;
    std::deque<MouseEvent> events;
public:
    Mouse():
        fd(open("/dev/input/mice", O_RDONLY))
    {
        if(fd == -1)
            throw std::runtime_error(std::string("can not use mice device: ") + strerror(errno));
    }
    bool empty() const noexcept
    {
        return events.size() == 0;
    }
    bool try_read() noexcept
    {
        int count = 0;
        struct pollfd pfd = {fd, POLLIN, 0};
        char event[3];
        while(poll(&pfd, 1, 0) > 0)
            if(read(fd, event, 3) == 3)
            {
                int x_rel = event[1];
                int y_rel = event[2];
                events.push_back(MouseEvent{x_rel, y_rel, event[0] & 0x1, event[0] & 0x2});
                count++;
            }
        return count != 0;
    }
    MouseEvent extract() noexcept
    {
        MouseEvent e = events.front();
        events.pop_front();
        return e;
    }
};
static Mouse mouse;
bool pollMouseEvent(MouseEvent &e) noexcept
{
    if(mouse.empty())
        if(!mouse.try_read())
            return false;
    e = mouse.extract();
    return true;
}
