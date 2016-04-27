#ifndef MODEL_UTIL
#define MODEL_UTIL

#include "Mesh.hpp"
#include "../common.hpp"

Mesh *Cube(const vec3 &pos, const vec3 &size, const vec3 &surface_color, const Material &material);
Mesh *Cube(const vec3 &surface_color, const Material &material);

Mesh *Plane(const vec3 &a, const vec3 &b, const vec3 &c, const vec3 &d, const vec3 &surface_color, const Material &material);
Mesh *Plane(const vec3 &surface_color, const Material &material);

#endif