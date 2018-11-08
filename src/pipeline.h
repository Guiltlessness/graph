#ifndef PIPELINE_H
#define PIPELINE_H

#include "geom.h"
#include "frame_buf.h"

class pipeline
{
public:
    vec3 campos;
    vec3 lighdir;
    mat3 lookat_ex;
    frame_buf *buf = nullptr;
    vertex vertex_shader(vertex const&) noexcept;
    pix fragment_shader(vertex const&) noexcept;
    void draw(vertex const*, size_t*, size_t) noexcept;
};

#endif // PIPELINE_H
