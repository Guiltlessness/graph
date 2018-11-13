#ifndef SPHERE_H
#define SPHERE_H

#include "frame_buf.h"

struct vrts_ans_inds {
    vertex* verts;
    size_t* inds;
    size_t len;
};

vrts_ans_inds create_sphere(float R, int num);


#endif // SPHERE_H
