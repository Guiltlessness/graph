#include "pipeline.h"
#include "geom.h"

#include <cmath>
#include <iostream>
#include <cwchar>

static const bool bfc_on = true;


pix mix(pix c1, pix c2, pix c3, float p, float q) {
    return pix{(unsigned char)(c1.b * (1 - p - q) + c2.b * p + c3.b * q),
            (unsigned char)(c1.g * (1 - p - q) + c2.g * p + c3.g * q),
            (unsigned char)(c1.r * (1 - p - q) + c2.r * p + c3.r * q),
            255};
}
//vec2 mix(vec2 const &v1, vec2 const &v2, vec2 const &v3, float p, float q) noexcept {
//    return vec2(v1.x * (1 - p - q) + v2.x * p + v3.x * q,
//                v1.y * (1 - p - q) + v2.y * p + v3.y * q);
//}
//vec3 mix(vec3 const &v1, vec3 const &v2, vec3 const &v3, float p, float q) noexcept {
//    return vec3(v1.x * (1 - p - q) + v2.x * p + v3.x * q,
//                v1.y * (1 - p - q) + v2.y * p + v3.y * q,
//                v1.z * (1 - p - q) + v2.z * p + v3.z * q);
//}
vertex mix(vertex const&v1, vertex const& v2, vertex const&v3, float p, float q) noexcept {
    float norm = (1 - p - q)/(v1.pos.z) + p / (v2.pos.z) + q / (v3.pos.z);
    return vertex{
        vec3(v1.pos.x * (1 - p - q) + v2.pos.x * p + v3.pos.x * q,
             v1.pos.y * (1 - p - q) + v2.pos.y * p + v3.pos.y * q,
             v1.pos.z * (1 - p - q) + v2.pos.z * p + v3.pos.z * q),
        vec2(v1.tex.x * (1 - p - q)/((v1.pos.z) * norm) + v2.tex.x * p / ((v2.pos.z) * norm) + v3.tex.x * q / ((v3.pos.z) * norm),
             v1.tex.y * (1 - p - q)/((v1.pos.z) * norm) + v2.tex.y * p / ((v2.pos.z) * norm) + v3.tex.y * q / ((v3.pos.z) * norm))
    };
}

void pipeline::lookat(vec3 const &e3, vec3 const &up) noexcept {
    const float tgalpha = 1.f;
    const float distance = 2.f;
    mat3 ans;
    vec3 e1 = normalize(cross(up, e3));
    vec3 e2 = normalize(cross(e3, e1));
    for(int i = 0; i < 3; ++i)
        ans.data[0][i] = e1.data[i] /*/ (-e3.data[i] + distance) * tgalpha*/;
    for(int i = 0; i < 3; ++i)
        ans.data[1][i] = e2.data[i] /*/ (-e3.data[i] + distance) * tgalpha*/;
    for(int i = 0; i < 3; ++i)
        ans.data[2][i] = e3.data[i];
    lookat_ex = ans;
}

pix texture_f(tga_t const &image, vec2 const &v) noexcept {
    uint16_t const w = image.width;
    uint16_t const h = image.height;
    pix ans = *(pix*)(image(v.y * (h - 1), v.x * (w - 1)));
    return ans;
}

vertex pipeline::vertex_shader(vertex const&ver) noexcept {
    vec3 ans = ver.pos;
    ans.z += eye.z;
    ans.y += eye.y;
    ans.x += eye.x;
    ans = lookat_ex * ans;
    ans.x /= ans.z;
    ans.y /= ans.z;
    return {ans, ver.tex};
}

pix pipeline::fragment_shader(vertex const& v) noexcept {
    return texture_f(texture, v.tex);
}
void pipeline::rasterize(vertex const& v1, vertex const& v2, vertex const& v3) noexcept {
    int w2 = (*buf).width / 2;
    int h2 = (*buf).height / 2;
    int v1_x = int(v1.pos.x * h2 + w2);
    int v2_x = int(v2.pos.x * h2 + w2);
    int v3_x = int(v3.pos.x * h2 + w2);
    int v1_y = int(v1.pos.y * h2 + h2);
    int v2_y = int(v2.pos.y * h2 + h2);
    int v3_y = int(v3.pos.y * h2 + h2);
    int x_min = std::max(0, std::min(v1_x, std::min(v2_x, v3_x)));
    int y_min = std::max(0, std::min(v1_y, std::min(v2_y, v3_y)));
    int x_max = std::min((*buf).width - 1, std::max(v1_x, std::max(v2_x, v3_x)));
    int y_max = std::min((*buf).height - 1, std::max(v1_y, std::max(v2_y, v3_y)));
    int det = (v2_x - v1_x) * (v3_y - v1_y) - (v3_x - v1_x) * (v2_y - v1_y);
    if(det == 0)
        return;
    for(int y = y_min; y < y_max; y++)
        for(int x = x_min; x < x_max; x++)
        {
            int det1 = (x - v1_x) * (v3_y - v1_y) - (v3_x - v1_x) * (y - v1_y);
            int det2 = (v2_x - v1_x) * (y - v1_y) - (x - v1_x) * (v2_y - v1_y);
            float p = float(det1) / det;
            float q = float(det2) / det;
            if(p < 0 || q < 0 || p + q > 1)
                continue;
            vertex v = mix(v1, v2, v3, p, q);
            if(z_test(x, y, v.pos.z)) {
                (*buf)[y][x] = fragment_shader(v);
            }
        }
}

static bool choose(int x1, int y1, int x2, int y2, int x_d, int y_d) noexcept {
    return std::abs((y_d * x1 - y1 * x_d) * x2) <
            std::abs((y_d * x2 - y2 * x_d) * x1);
}

static void make_line(frame_buf* buf, vertex const&v1, vertex const&v2) noexcept {
    int w2 = (*buf).width / 2;
    int h2 = (*buf).height / 2;
    int v1_x = int(v1.pos.x * h2 + w2);
    int v2_x = int(v2.pos.x * h2 + w2);
    int v1_y = int(v1.pos.y * h2 + h2);
    int v2_y = int(v2.pos.y * h2 + h2);
    int x_min = std::max(0, std::min(v1_x, v2_x));
    int y_min = std::max(0, std::min(v1_y, v2_y));
    int x_max = std::min((*buf).width - 1, std::max(v1_x, v2_x));
    int y_max = std::min((*buf).height - 1, std::max(v1_y, v2_y));
    int x_d = v2_x - v1_x;
    int y_d = v2_y - v1_y;
    int move_x = 1;
    int move_y = 1;
    if (x_d < 0)
        move_x *= -1;
    if (y_d < 0)
        move_y *= -1;
    for(int x = v1_x, y = v1_y; x >= x_min && x <= x_max && y >= y_min && y <= y_max; ){
        (*buf)[y][x] = pix(255,0,255,255);
        if ( choose(x + move_x - v1_x, y - v1_y, x - v1_x, y - v1_y + move_y, x_d, y_d))
            x += move_x;
        else
            y += move_y;
    }
}

void pipeline::draw(vertex const *verts, size_t const *inds, size_t size) noexcept
{
    for(size_t i = 0u; i < size; i += 3)
        bfculling
            (
                vertex_shader(verts[inds[i]]),
                vertex_shader(verts[inds[i + 1]]),
                vertex_shader(verts[inds[i + 2]])
            );
}

void pipeline::bfculling(vertex const& v1, vertex const& v2, vertex const& v3) noexcept {
    if (bfc_on) {
        bool isclockwise = ((v1.pos.x - v3.pos.x) * (v2.pos.y - v1.pos.y) - (v2.pos.x - v1.pos.x) * (v1.pos.y - v3.pos.y)) > 0;
        if (isclockwise) {
            rasterize(v1, v2, v3);
        }
    } else {
        rasterize(v1, v2, v3);
    }
}

pipeline::pipeline(tga_t const& img, frame_buf* frb)
    : eye(0.f, 0.f, 3.f)
    , texture(img)
    , buf(frb)
{
    z_buffer = new float[buf->high_v * buf->lenght_v];
    float inf = INFINITY;
    wmemset((wchar_t*)z_buffer, *(wchar_t*)&inf, buf->high_v * buf->lenght_v);
}

void pipeline::clear() noexcept {
    float inf = INFINITY;
    wmemset((wchar_t*)z_buffer, *(wchar_t*)&inf, buf->high_v * buf->lenght_v);
    buf->clear();
}

bool pipeline::z_test(int x, int y, float z) noexcept {
    float &comp = *(z_buffer + y * buf->lenght_v + x);
    if (comp >= z) {
        comp = z;
        return true;
    }
    return false;
}

pipeline::~pipeline() {
    delete [] z_buffer;
}

void pipeline::update() const noexcept {
    buf->update();
}


void pipeline::draw_lines(vertex const*vs, size_t const*sizes, size_t len) noexcept {
    for(uint i = 0; i < len; i += 3) {
        make_line(buf,
                  vertex_shader(vs[sizes[i]]),
                  vertex_shader(vs[sizes[i + 1]]));
        make_line(buf,
                  vertex_shader(vs[sizes[i + 1]]),
                  vertex_shader(vs[sizes[i + 2]]));
        make_line(buf,
                  vertex_shader(vs[sizes[i]]),
                 vertex_shader(vs[sizes[i + 2]]));
    }
}
