#include "Cube.hpp"

bool Cube::intersect(const vec3 &position, const vec3 &direction, float &t, vec3 &hit_normal, vec3 &hit_surface_color, BasicModel const* &hit_model) const {

	int idx;
	if (!box.intersect(position, direction, t, idx))
		return false;

	hit_normal = box.getNormal(idx);
	hit_surface_color =  surface_color;
	hit_model = this;

	return true;
}