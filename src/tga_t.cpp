#include "tga_t.h"
extern "C" {
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
}
#include <stdexcept>
#include <string>
#include <utility>
#include <iostream>


tga_t::tga_t(char const* fileloc)
{
    int file = open(fileloc, O_RDONLY);
    if (file < 0)
        throw std::runtime_error(std::string("The image file:") + fileloc + " cannot to be opened\n" + strerror(errno));
    u_int8_t info_buf[18];
    if(read(file, info_buf, 18) < 0)
        throw std::runtime_error(std::string("Cannot read") + fileloc + "\n" + strerror(errno));

    parse_info(info_buf);

    try {
        data = new u_int8_t[height * width * 4]; // по хорошему сделать ... * bpp / 8
        if(read(file, data, height * width * 4) < 0)
            throw std::runtime_error(std::string("Cannot read") + fileloc + "\n" + strerror(errno));
    } catch (std::exception e) {
        close(file);
        throw e;
    }
    close(file);
}

void tga_t::parse_info(uint8_t const* info) noexcept {
    memcpy(& header, info, 3);                  info += 3;
    memcpy(&(header.map_description), info, 5); info += 5;
    memcpy(&(header.img_description), info, 10);
}

tga_t::~tga_t() {
    delete [] data;
}

decltype (std::declval<tga_t>().header) const& tga_t::head_info() const noexcept {
    return header;
}

u_int8_t * tga_t::operator()(float y, float x) const noexcept {
    return data + (int(y) * width + int(x)) * 4;  // image dependent
}

