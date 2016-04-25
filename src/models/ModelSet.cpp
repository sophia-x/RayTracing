#include "ModelSet.hpp"

bool ModelSet::intersect(const vec3 &position, const vec3 &direction, const vec3 &inv_direction, float &t, vec3 &hit_normal, vec3 &hit_surface_color, BasicModel const* &hit_model) const {
	return bbox.intersect(position, direction, inv_direction, t, hit_normal, hit_surface_color, hit_model);
}