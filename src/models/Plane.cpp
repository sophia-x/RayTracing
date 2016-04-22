#include "Plane.hpp"
#include "../utils/intersect_tool.hpp"

const float Plane::MAX = 10000;

bool Plane::intersect(const vec3 &position, const vec3 &direction, float &t, vec3 &hit_normal, vec3 &hit_surface_color, BasicModel const* &hit_model) const {
	if (!plane_intersect(position, direction, normal, d, t))
		return false;

	hit_normal = normal;
	hit_surface_color = surface_color;
	hit_model = this;

	return true;
}