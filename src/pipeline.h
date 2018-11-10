#ifndef PIPELINE_H
#define PIPELINE_H

#include "geom.h"
#include "frame_buf.h"
#include "tga_t.h"

class pipeline
{
    vertex vertex_shader(vertex const&) noexcept;
    pix fragment_shader(vertex const&) noexcept;
    void rasterize(vertex const&, vertex const&, vertex const&) noexcept;
    bool z_test(int x, int y, float z) noexcept;

    float* z_buffer;
public:
    vec3 campos;
    vec3 lighdir;
    mat3 lookat_ex;
    tga_t const& texture;
    frame_buf *buf = nullptr;
    void draw(vertex const*, size_t const*, size_t) noexcept;
    void clear() noexcept;

    pipeline(tga_t const&, frame_buf* frb);
    ~pipeline();
};

#endif // PIPELINE_H
