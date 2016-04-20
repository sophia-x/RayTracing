#include <cmath>

#include "Cube.hpp"

using namespace std;

const Box Cube::box(vec3(0), vec3(1));

bool Cube::intersect(const vec4 &position, const vec4 &direction, float &t, vec4 &hit_normal, vec3 &hit_surface_color) const {
	vec4 position_model = world2model_matrix * position;
	vec4 direction_model = normalize(world2model_matrix * direction);

	int idx;
	if (!box.intersect(position_model, direction_model, t, idx))
		return false;

	hit_normal = normalize(vec4(vec3(normal2world_matrix * box.getNormal(idx)), 0));
	hit_surface_color =  surface_color;
	t = length(model2world_matrix * (t * direction_model));

	return true;
}