#include <cmath>

#include "Cube.hpp"

using namespace std;

bool Cube::intersect(const vec3 &position, const vec3 &direction, float &t, vec3 &hit_normal, vec3 &hit_surface_color) const {

	int idx;
	if (!box.intersect(position, direction, t, idx))
		return false;

	hit_normal = box.getNormal(idx);
	hit_surface_color =  surface_color;

	return true;
}