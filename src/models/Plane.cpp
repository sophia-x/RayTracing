#include <limits>

#include "Plane.hpp"
#include "../utils/intersect_tool.hpp"

using namespace std;

bool Plane::intersect(const vec4 &position, const vec4 &direction, float &t, vec4 &hit_normal, vec3 &hit_surface_color) const {
	vec4 position_model = world2model_matrix * position;
	vec4 direction_model = normalize(world2model_matrix * direction);
	vec4 n(0, 1, 0, 0);

	if (!plane_intersect(position_model, direction_model, n, 0, vec4(-0.5, 0.5, -0.5, 0.5), 0, 2, t))
		return false;

	t = length(model2world_matrix * (t * direction_model));
	hit_normal = normalize(vec4(vec3(normal2world_matrix * vec4(0, 1, 0, 0)), 0));
	hit_surface_color = surface_color;

	return true;
}