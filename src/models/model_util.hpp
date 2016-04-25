#ifndef MODEL_UTIL

#include "Mesh.hpp"

#include "../common.hpp"

// BasicModel *Cube();
Mesh *Plane(const vec3 &a, const vec3 &b, const vec3 &c, const vec3 &d, const vec3 &surface_color, const Material &material);
Mesh *Plane(const vec3 &surface_color, const Material &material);

#define MODEL_UTIL
#endif