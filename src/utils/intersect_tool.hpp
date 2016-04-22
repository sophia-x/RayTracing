#ifndef INTERSECT_TOOL
#define INTERSECT_TOOL

#include <glm/glm.hpp>
#include <vector>

using namespace glm;
using namespace std;

bool plane_intersect(const vec3 &position, const vec3 &direction, const vec3 &normal, float d, float &t);

#endif