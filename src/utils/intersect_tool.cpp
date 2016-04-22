#include "intersect_tool.hpp"
#include "../common.hpp"

using namespace std;

bool plane_intersect(const vec3 &position, const vec3 &direction, const vec3 &normal, float d, float &t) {
	float nd = dot(normal, direction);

	if (abs(nd) <= PLANE_EPSILON) {
		t = nd > 0 ? numeric_limits<float>::max() : -numeric_limits<float>::max();
		return false;
	}

	t = -(d + dot(normal, position)) / nd;
	if (t < 0)
		return false;

	return true;
}