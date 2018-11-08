#include "geom.h"

#include <cassert>
#include <cmath>

mat3::mat3()
    : data {1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f}
{}
mat3::mat3(mat3 const& oth) {
    for(int i = 0; i < 3; ++i)
        for(int j = 0; j < 3; ++j)
        data[i][j] = oth.data[i][j];
}
mat3 mat3::operator*(mat3 const& oth) const noexcept
{
    mat3 ans;
    for (int x = 0; x != 3; ++x)
        for (int y = 0; y != 3; ++y)
            for (int k = 0; k < 3; ++k)
            {
                ans[x][y] += oth[x][k] * data[k][y];
            }
    return ans;
}

float* mat3::operator[](int index) noexcept {
    assert(index < 3);
    return data[index];
}

float const* mat3::operator[](int index) const noexcept {
    assert(index < 3);
    return data[index];
}

vec3 normalize(vec3 const &v) noexcept {
    float len = std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    return {v.x / len, v.y / len, v.z / len};
}

vec3 cross(vec3 const&v1, vec3 const&v2) noexcept {
    return {v1.y* v2.z - v1.z * v2.y, -(v1.x * v2.z - v1.z * v2.x), v1.x * v2.y - v1.y * v2.x};
}

mat3 lookat(vec3 const &e3, vec3 const &up) noexcept {
    mat3 ans;
    vec3 e1 = normalize(cross(up, e3));
    vec3 e2 = normalize(cross(e3, e1));
    for(int i = 0; i < 3; ++i)
        ans.data[0][i] = e1.data[i];
    for(int i = 0; i < 3; ++i)
        ans.data[1][i] = e2.data[i];
    for(int i = 0; i < 3; ++i)
        ans.data[2][i] = e3.data[i];
    return ans;
}

vec3 operator*(mat3 const&m, vec3 const&v) noexcept {
    vec3 out;
    for(int i = 0; i < 3; i++)
    {
        out.data[i] = 0.f;
        for(int j = 0; j < 3; j++)
            out.data[i] += m.data[i][j] * v.data[j];
    }
    return out;
}
