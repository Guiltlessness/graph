#include "sphere.h"

#include <map>
#include <vector>
#include <cmath>


struct edge {
    size_t from;
    size_t until;
    edge(size_t v1, size_t v2)
        : from(std::min(v1, v2))
        , until(std::max(v1, v2))
    {}
    bool operator<(edge const& e) const noexcept {
        if (from == e.from)
            return until < e.until;
        return from < e.from;
    }
};

static void add(std::map<edge, size_t>& edges, std::vector<vertex>& data, vertex const& v1, vertex const&v2, edge const& e_12) {
    if(!edges.count(e_12)) {
        edges[e_12] = data.size();
        vertex add = {{(v1.pos/2 + v2.pos/2)},
                      {v1.tex.x/2 + v2.tex.x/2, v1.tex.y/2 + v2.tex.y/2}};
        data.push_back(add);
    }
}

vrts_ans_inds create_sphere(float R, int num) {
    std::vector<vertex> data{
        {{ 1.f, 0.f, 0.f}, {0.5f, 0.5f}}, // 0
        {{-1.f, 0.f, 0.f}, {0.5f, 0.5f}}, // 1
        {{ 0.f, 1.f, 0.f}, {0.5f, 1.f}}, // 2
        {{ 0.f,-1.f, 0.f}, {0.5f, 0.f}}, // 3
        {{ 0.f, 0.f, 1.f}, {0.1f, 0.5f}}, // 4
        {{ 0.f, 0.f,-1.f}, {0.9f, 0.5f}}, // 5
    };
    std::vector<size_t> queue{
        0,2,5,
        0,5,3,
        0,4,2,
        4,1,2,
        0,3,4,
        3,1,4,
        3,5,1,
        1,5,2
    };
    std::map<edge, size_t> edges;
    auto check_add = [&data, &edges](vertex const& v1, vertex const&v2, edge const& e_12) mutable {
        return add(edges, data, v1, v2, e_12);
    };

    std::vector<size_t> next_queue;
    for(int depth = 0; depth < num; ++depth)
    {
        next_queue.reserve(queue.size() * 4);
        data.reserve(data.size() * 2);

        size_t s = queue.size();
        for(size_t i = 0; i < s; i += 3) {
            vertex const& v1 = data[queue[i]];
            vertex const& v2 = data[queue[i + 1]];
            vertex const& v3 = data[queue[i + 2]];
            edge e_12(queue[i], queue[i + 1]);
            edge e_32(queue[i + 2], queue[i + 1]);
            edge e_31(queue[i + 2], queue[i]);
            check_add(v1, v2, e_12);
            check_add(v3, v2, e_32);
            check_add(v3, v1, e_31);

            size_t ind_12 = edges.at(e_12);
            size_t ind_32 = edges.at(e_32);
            size_t ind_31 = edges.at(e_31);

            next_queue.push_back(queue[i]);     next_queue.push_back(ind_12);           next_queue.push_back(ind_31);
            next_queue.push_back(ind_31);       next_queue.push_back(ind_12);           next_queue.push_back(ind_32);
            next_queue.push_back(ind_12);       next_queue.push_back(queue[i + 1]);     next_queue.push_back(ind_32);
            next_queue.push_back(ind_31);       next_queue.push_back(ind_32);           next_queue.push_back(queue[i + 2]);
        }
        edges.clear();
        queue = std::move(next_queue);
    }

    for(auto& v : data) {
        float len = std::sqrt(
                    v.pos.x * v.pos.x +
                    v.pos.y * v.pos.y +
                    v.pos.z * v.pos.z);
        float coef = R / len;
        for(int i = 0; i < 3; ++i)
            v.pos[i] *= coef;
    }

    vertex *vrts = new vertex[data.size()];
    size_t* queue__ = new size_t[queue.size()];

    int i = 0;
    for(auto& v : data) {
        vrts[i++] = v;
    }
    i = 0;
    for(auto& q : queue) {
        queue__[i++] = q;
    }
    return {vrts, queue__, queue.size()};
}
