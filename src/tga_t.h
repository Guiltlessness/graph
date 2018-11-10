#ifndef TGA_T_H
#define TGA_T_H

#include <cstdint>

class tga_t
{
    uint8_t* data;
    struct {
        uint8_t id_len;
        uint8_t color_map_t;
        uint8_t image_t;
        struct {
            uint16_t frtelm_idx;
            uint16_t clrmap_len;
            uint8_t  map_size;
        } map_description;
        struct {
            uint16_t x_pos;
            uint16_t y_pos;
            uint16_t width;
            uint16_t height;
            uint8_t  bits_per_pixel;
            uint8_t some_descr;
        } img_description;
    } header;

    void parse_info(uint8_t const* info) noexcept;
public:
    uint16_t const& width = header.img_description.width;
    uint16_t const& height = header.img_description.height;

    tga_t() = delete;
    tga_t(char const* filelocation);
    uint8_t* operator()(float y, float x) const noexcept;
    decltype (header) const& head_info() const noexcept;
    ~tga_t();
};

#endif // TGA_T_H
