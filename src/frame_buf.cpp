#include "frame_buf.h"

pix::pix() {
    b = g = 100;
    a = r = 255;
}

pix::pix(uc a1, uc a2, uc a3, uc a4)
    : b(a1)
    , g(a2)
    , r(a3)
    , a(a4)
{}

frame_buf::frame_buf() {
    fb = open("/dev/fb0", O_WRONLY);
    struct fb_fix_screeninfo fsinfo;
    if(ioctl(fb, FBIOGET_FSCREENINFO, &fsinfo) < 0) {
        write(1, "Хуита какаято\n", 27);
    };
    struct fb_var_screeninfo vsinfo;
    width = fsinfo.line_length / 4;
    height = fsinfo.smem_len /fsinfo.line_length;
    if(ioctl(fb, FBIOGET_VSCREENINFO, &vsinfo) < 0) {
        write(1, "Хуита какаято\n", 27);
    };
    high_v = vsinfo.yres /*/ vsinfo.bits_per_pixel*/;
    lenght_v = vsinfo.xres /*/ vsinfo.bits_per_pixel*/;
    content = new pix[height * width];
    close(fb);
}
frame_buf::~frame_buf() {
    delete[] content;
}

pix* frame_buf::operator[] (int y_ind) noexcept {
    return content + width * (high_v - y_ind - 1);
}

void frame_buf::set_pixel(int x, int y, pix const& p) noexcept {
    (*this)[y][x] = p;
}

void frame_buf::clear() noexcept {
    wmemset((wchar_t*)content, 0, height * width);
}

void frame_buf::update() noexcept {
    int fb = open("/dev/fb0", O_WRONLY);
    write(fb, content, height * width * 4);
    close(fb);
}

bool operator < (vertex const& v1, vertex const& v2) {
    int i = 0;
    for( ; i < 3 && (v1.pos[i] == v2.pos[i]); ++i);
    return v1.pos[i] < v2.pos[i];
}
