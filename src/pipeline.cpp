#include "pipeline.h"
#include "geom.h"

#include <cmath>
#include <iostream>


pix mix(pix c1, pix c2, pix c3, float p, float q)
{
    return pix{(unsigned char)(c1.b * (1 - p - q) + c2.b * p + c3.b * q),
            (unsigned char)(c1.g * (1 - p - q) + c2.g * p + c3.g * q),
            (unsigned char)(c1.r * (1 - p - q) + c2.r * p + c3.r * q),
            255};
}

vertex pipeline::vertex_shader(vertex const&ver) noexcept {
    return {lookat_ex * ver.pos, ver.c};
}

pix pipeline::fragment_shader(vertex const& v) noexcept {
    return v.c;
}
void pipeline::draw(vertex const* v, size_t* indxs, size_t fr_sz) noexcept {
    size_t ind = 0;
    for (; ind != fr_sz; ind += 3) {
        int w2 = (*buf).lenght / 2;
        int h2 = (*buf).height / 2;
        vertex v1 = vertex_shader(v[indxs[ind + 0]]);
        vertex v2 = vertex_shader(v[indxs[ind + 1]]);
        vertex v3 = vertex_shader(v[indxs[ind + 2]]);
        int v1_x = v1.pos.x * w2 + w2;
        int v2_x = v2.pos.x * w2 + w2;
        int v3_x = v3.pos.x * w2 + w2;
        int v1_y = v1.pos.y * h2 + h2;
        int v2_y = v2.pos.y * h2 + h2;
        int v3_y = v3.pos.y * h2 + h2;
        int x_min = std::max(0, std::min(v1_x, std::min(v2_x, v3_x)));
        int y_min = std::max(0, std::min(v1_y, std::min(v2_y, v3_y)));
        int x_max = std::min((*buf).lenght - 1, std::max(v1_x, std::max(v2_x, v3_x)));
        int y_max = std::min((*buf).height - 1, std::max(v1_y, std::max(v2_y, v3_y)));
        int det = (v2_x - v1_x) * (v3_y - v1_y) - (v3_x - v1_x) * (v2_y - v1_y);
        if(det == 0)
            return;
        for(int y = y_min; y < y_max; y++)
            for(int x = x_min; x < x_max; x++)
            {
                int det1 = (x - v1_x) * (v3_y - v1_y) - (v3_x - v1_x) * (y - v1_y);
                int det2 = (v2_x - v1_x) * (y - v1_y) - (x - v1_x) * (v2_y - v1_y);
                float p = (float)det1 / det;
                float q = (float)det2 / det;
                if(p < 0 || q < 0 || p + q > 1)
                    continue;
                (*buf)[y][x] = mix(v1.c, v2.c, v3.c, p, q);
            }
    }
}
