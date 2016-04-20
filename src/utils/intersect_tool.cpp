#include "intersect_tool.hpp"

using namespace std;

bool plane_intersect(const vec4 &position, const vec4 &direction, const vec4 &normal, float d, const vec4 &border, int idx1, int idx2, float &t) {
	float nd = dot(normal, direction);
	if (abs(nd) <= numeric_limits<float>::epsilon())
		return false;

	t = -(d + dot(normal, position)) / nd;
	if (t < 0)
		return false;

	vec4 p = position + t * direction;
	if (p[idx1] < border[0] || p[idx1] > border[1] || p[idx2] < border[2] || p[idx2] > border[3])
		return false;

	return true;
}