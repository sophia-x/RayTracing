#ifndef INTERSECT_TOOL
#define INTERSECT_TOOL

#include "../common.hpp"

bool plane_intersect(const vec3 &position, const vec3 &direction, const vec3 &normal, float d, float &t);

#endif