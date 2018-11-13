
#ifndef GEOM_H
#define GEOM_H

struct mat3 {
    float data[3][3];

    mat3();
    mat3(mat3 const& oth) ;
    float* operator[] (int index) noexcept ;
    const float* operator[] (int index) const noexcept;

    mat3 operator*(mat3 const& oth) const noexcept;

};
struct vec2
{
    float x, y;
    vec2();
    vec2(float _x, float _y);
};
struct vec3 {
    union
    {
        float data[3];
        struct
        {
            float x, y, z;
        };
    };
    vec3() noexcept;
    vec3(float, float, float) noexcept;
    float &operator[](int i) noexcept;
    float const &operator[](int i) const noexcept;
    vec3 operator+(vec3 const &) const noexcept;
    vec3 operator-(vec3 const &) const noexcept;
    vec3 operator*(float) const noexcept;
    vec3 operator/(float) const noexcept;
};

vec3 cross(vec3 const&v1, vec3 const&v2) noexcept;
float dot (vec3 const&v1, vec3 const&v2) noexcept;
mat3 lookat(vec3 const &dir, vec3 const &up) noexcept;
vec3 normalize(vec3 const &v) noexcept ;
vec3 operator*(mat3 const&m, vec3 const&v) noexcept;



#endif // GEOM_H
