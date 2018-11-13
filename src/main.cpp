#include "frame_buf.h"
#include "pipeline.h"
#include "tga_t.h"
#include "mouse.h"
#include "sphere.h"

#include <iostream>
#include <cmath>
#include <list>
#include <utility>
#include <map>
#include <vector>

#include <chrono>
#include <thread>


frame_buf context;
static vertex verts[] =
{
    {{-0.5f, -0.5f,  0.5f}, {0.f, 0.f}}, // 0
    {{-0.5f,  0.5f,  0.5f}, {0.f, 1.f}}, // 1
    {{ 0.5f, -0.5f,  0.5f}, {1.f, 0.f}}, // 2
    {{ 0.5f,  0.5f,  0.5f}, {1.f, 1.f}}, // 3
    {{-0.5f, -0.5f, -0.5f}, {1.f, 1.f}}, // 4
    {{-0.5f,  0.5f, -0.5f}, {1.f, 0.f}}, // 5
    {{ 0.5f, -0.5f, -0.5f}, {0.f, 1.f}}, // 6
    {{ 0.5f,  0.5f, -0.5f}, {0.f, 0.f}}, // 7
};
static size_t inds[] = {
    0, 1, 2,
    1, 3, 2,
    2, 3, 7,
    7, 6, 2,
    3, 1, 5,
    5, 7, 3,
    6, 7, 5,
    5, 4, 6,
    5, 4, 0,
    5, 0, 1,
    0, 2, 6,
    0, 6, 4
};

int main()
{
    tga_t img("yoba.tga");

    pipeline pl(img, &context);
    float phi = 0.f, theta = 0.f;
    auto data = create_sphere(1.f, 4);
    while(true)
    {
        pl.clear();

        MouseEvent e;
        while(pollMouseEvent(e))
        {
            phi += e.xrel / 300.f;
            theta += e.yrel / 400.f;
        }
        pl.lookat_ex = lookat({std::sin(phi) * std::cos(theta), std::sin(theta), std::cos(phi) * std::cos(theta)} , {0, 1, 0});
        pl.draw(data.verts, data.inds, data.len); // sphere
//        pl.draw(verts, inds, sizeof (inds) / sizeof (size_t)); // cube
//        pl.draw_lines(data.verts, data.inds, s);
        pl.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }

    return 0;
}
