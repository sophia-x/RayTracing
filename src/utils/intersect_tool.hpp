#ifndef INTERSECT_TOOL
#define INTERSECT_TOOL

#include <glm/glm.hpp>
#include <vector>

using namespace glm;
using namespace std;

bool plane_intersect(const vec4 &position, const vec4 &direction, const vec4 &normal, float d, const vec4 &border, int idx1, int idx2, float &t);

#endif