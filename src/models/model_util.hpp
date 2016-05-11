#ifndef MODEL_UTIL
#define MODEL_UTIL

#include "Mesh.hpp"
#include "../common.hpp"

Mesh *Cube(const vec3 &pos, const vec3 &size, const Material &material);
Mesh *Cube(const Material &material);

Mesh *Plane(const vec3 &a, const vec3 &b, const vec3 &c, const vec3 &d, const Material &material);
Mesh *Plane(const vec3 &a, const vec3 &b, const vec3 &c, const vec3 &d, const vec2 uv_a, const vec2 uv_b, const vec2 uv_c, const vec2 uv_d, const Material &material);
Mesh *Plane(const vec2 uv_a, const vec2 uv_b, const vec2 uv_c, const vec2 uv_d, const Material &material);
Mesh *Plane(const Material &material);

#endif