#ifndef PIPELINE_H
#define PIPELINE_H

#include "geom.h"
#include "frame_buf.h"
#include "tga_t.h"

class pipeline
{

    float* z_buffer;
    mat3 lookat_ex;

    vertex vertex_shader(vertex const&) noexcept;
    pix fragment_shader(vertex const&) noexcept;
    void rasterize(vertex const&, vertex const&, vertex const&) noexcept;
    bool z_test(int x, int y, float z) noexcept;
    void bfculling(vertex const&, vertex const&, vertex const&) noexcept;

public:
    vec3 eye;
    vec3 movement;
    vec3 campos;
    vec3 lighdir;
    tga_t const& texture;
    frame_buf *buf = nullptr;
    void draw_lines(vertex const*, size_t const*, size_t) noexcept;
    void draw(vertex const*, size_t const*, size_t) noexcept;
    void clear() noexcept;
    void update() const noexcept;
    pipeline(tga_t const&, frame_buf* frb);
    void lookat(vec3 const &e3, vec3 const &up) noexcept;
    ~pipeline();
};

#endif // PIPELINE_H
