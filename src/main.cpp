#include "frame_buf.h"
#include "pipeline.h"
#include "tga_t.h"
#include "mouse.h"

#include <iostream>
#include <cmath>

frame_buf context;
static vertex verts[] =
{
    {{-0.5f, -0.5f,  0.5f}, {0.f, 0.f}}, // 0
    {{-0.5f,  0.5f,  0.5f}, {0.f, 1.f}}, // 1
    {{ 0.5f, -0.5f,  0.5f}, {1.f, 0.f}}, // 2
    {{ 0.5f,  0.5f,  0.5f}, {1.f, 1.f}}, // 3
    {{-0.5f, -0.5f, -0.5f}, {0.f, 0.f}}, // 4
    {{-0.5f,  0.5f, -0.5f}, {0.f, 1.f}}, // 5
    {{ 0.5f, -0.5f, -0.5f}, {1.f, 0.f}}, // 6
    {{ 0.5f,  0.5f, -0.5f}, {1.f, 1.f}}, // 7
//    {{-0.5f, -0.5f,  0.5f}, {1.f, 0.f}}, // 8 (0)
//    {{-0.5f,  0.5f,  0.5f}, {1.f, 1.f}}, // 9 (1)
//    {{-0.5f,  0.5f, -0.5f}, {0.f, 1.f}}, // 10(5)
//    {{-0.5f, -0.5f, -0.5f}, {0.f, 0.f}}, // 11(4)
//    {{ 0.5f, -0.5f,  0.5f}, {0.f, 0.f}}, // 12(2)
//    {{ 0.5f,  0.5f,  0.5f}, {0.f, 1.f}}, // 13(3)
//    {{ 0.5f, -0.5f, -0.5f}, {1.f, 0.f}}, // 14(6)
//    {{ 0.5f,  0.5f, -0.5f}, {1.f, 1.f}}, // 15(7)
};
//size_t inds[] = {0, 1, 2, 1, 2, 3, 4, 5, 6, 5, 6, 7, 12, 13, 15, 12, 14, 15, 8, 9, 10, 8, 11, 10};
static size_t inds[] = {0,2,1,1,2,3,1,3,5,5,3,7,2,6,3,3,6,7,1,5,4,4,0,1,0,4,6,6,2,0,4,5,7,7,6,4};

int main()
{
    for(int i = 0; i != (sizeof (verts) / sizeof (vertex)); ++i)
    {
        verts[i].pos.x -= 1.f;
    }
    tga_t img("yoba.tga");

    pipeline pl(img, &context);
    float phi = 0.f, theta = 0.f;
    while(true)
    {
        MouseEvent e;
        while(pollMouseEvent(e))
        {
            phi += e.xrel / 300.f;
            theta += e.yrel / 400.f;
        }
        pl.lookat_ex = lookat({std::sin(phi) * std::cos(theta), std::sin(theta), std::cos(phi) * std::cos(theta)} , {0, 1, 0});
//        phi += 0.01f;
        pl.draw(verts, inds, sizeof(inds) / sizeof(size_t));
        pl.update();
        pl.clear();
    }

    return 0;
}
