#include "frame_buf.h"
#include "pipeline.h"
#include "tga_t.h"

#include <iostream>
#include <cmath>

frame_buf context;
vertex verts[] =
{
    {{-0.5f, -0.5f,  0.5f}, {0.f, 0.f}}, // 0
    {{-0.5f,  0.5f,  0.5f}, {0.f, 1.f}}, // 1
    {{ 0.5f, -0.5f,  0.5f}, {1.f, 0.f}}, // 2
    {{ 0.5f,  0.5f,  0.5f}, {1.f, 1.f}}, // 3
    {{-0.5f, -0.5f, -0.5f}, {0.f, 0.f}}, // 4
    {{-0.5f,  0.5f, -0.5f}, {0.f, 1.f}}, // 5
    {{ 0.5f, -0.5f, -0.5f}, {1.f, 0.f}}, // 6
    {{ 0.5f,  0.5f, -0.5f}, {1.f, 1.f}}, // 7
    {{-0.5f, -0.5f,  0.5f}, {1.f, 0.f}}, // 8 (0)
    {{-0.5f,  0.5f,  0.5f}, {1.f, 1.f}}, // 9 (1)
    {{-0.5f,  0.5f, -0.5f}, {0.f, 1.f}}, // 10(5)
    {{-0.5f, -0.5f, -0.5f}, {0.f, 0.f}}, // 11(4)
    {{ 0.5f, -0.5f,  0.5f}, {0.f, 0.f}}, // 12(2)
    {{ 0.5f,  0.5f,  0.5f}, {0.f, 1.f}}, // 13(3)
    {{ 0.5f, -0.5f, -0.5f}, {1.f, 0.f}}, // 14(6)
    {{ 0.5f,  0.5f, -0.5f}, {1.f, 1.f}}, // 15(7)
};
size_t inds[] = {0, 1, 2, 1, 2, 3, 4, 5, 6, 5, 6, 7, 12, 13, 15, 12, 14, 15, 8, 9, 10, 8, 11, 10};

int main()
{
    for(int i = 0; i != (sizeof (verts) / sizeof (vertex)); ++i)
    {
        verts[i].pos.x -= 1.f;
//        verts[i].pos.y += 2.f;
    }
    tga_t img("yoba.tga");

    pipeline pl(img, &context);
//    std::cout << int(img.head_info().img_description.bits_per_pixel) << std::endl;
//    pl.buf = &context;
    float phi_ = 0.f, phi = std::sin(phi_),  theta_ = -0.4f, theta = 0.f;
    while(phi_ < 10.f)
    {
        pl.lookat_ex = lookat({std::sin(phi_) * std::cos(theta), std::sin(theta), std::cos(phi_) * std::cos(theta)} , {0, 1, 0});
        phi_ += 0.01f;
//        phi = std::sin(phi_); theta = theta_;
        pl.draw(verts, inds, sizeof(inds) / sizeof(size_t));
        pl.buf->update();
        pl.clear();
    }

    return 0;
}
