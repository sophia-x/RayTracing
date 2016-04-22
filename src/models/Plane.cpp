#include "Plane.hpp"
#include "../utils/intersect_tool.hpp"

bool Plane::intersect(const vec3 &position, const vec3 &direction, float &t, vec3 &hit_normal, vec3 &hit_surface_color) const {
	if (!plane_intersect(position, direction, normal, d, t))
		return false;

	hit_normal = normal;
	hit_surface_color = surface_color;

	return true;
}