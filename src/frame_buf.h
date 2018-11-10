#ifndef FRAME_BUF_H
#define FRAME_BUF_H

extern "C"
{
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <wchar.h>
}
#include <algorithm>

#include "geom.h"



//b g r a
struct pix {
    unsigned char b, g, r, a;
    pix();
    typedef unsigned char uc;
    pix(uc a1, uc a2, uc a3, uc a4);
};

struct vertex
{
    vec3 pos;
    vec2 tex;
};


struct frame_buf {
    pix* content;
    int height, width, high_v, lenght_v, fb;
    frame_buf() ;
    ~frame_buf () ;
    pix* operator[] (int y_ind) noexcept;
    void set_pixel(int x, int y, pix const& p) noexcept;
    void clear() noexcept;
    void update() noexcept;

};

#endif // FRAME_BUF_H
