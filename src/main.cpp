#include "frame_buf.h"
#include "pipeline.h"

#include <cmath>

frame_buf context;


int main()
{

    vertex v1{{-0.5f, -0.5f, 0.f}, {255, 0, 0, 255}};
    vertex v2{{0.5f, -0.5f, 0.f},  {0, 255, 0, 255}};
    vertex v3{{0.5f, 0.5f, 0.f},    {0, 0, 255, 255}};
    vertex v4{{-0.5f,0.5f, 0.f}, {255, 128, 128, 255}};

    vertex vs[4] = {v1, v2, v3, v4};
    size_t s[6] = {0, 1, 2, 0, 2, 3};

    pipeline pl;
    pl.buf = &context;
    float phi = 0.f, theta = 0.4f;
    while(1)
    {
        pl.lookat_ex = lookat({std::sin(phi) * std::cos(theta), std::sin(theta), std::cos(phi) * std::cos(theta)} , {0, 1, 0});
        phi += 0.01f; theta += 0.03f;
        pl.draw(vs, s, 6);
        pl.buf->update();
        pl.buf->clear();
    }


    return 0;
}
