#include <limits>

#include "Plane.hpp"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

using namespace std;

bool Plane::intersect(const vec4 &position, const vec4 &direction, float &t, vec4 &hit_normal, vec3 &hit_surface_color) const {
	vec4 position_model = world2model_matrix * position;
	vec4 direction_model = normalize(world2model_matrix * direction);
	vec4 n(0, 1, 0, 0);

	float dir_dot = dot(direction_model, n);
	float uni_dir_dot = position_model[1] >= 0 ? -dir_dot : dir_dot;
	if (uni_dir_dot <= numeric_limits<float>::epsilon())
		return false;

	t = abs(position_model[1]) / uni_dir_dot;
	vec4 hit_pos = position_model + direction_model * t;
	if (abs(hit_pos[0]) > 0.5 || abs(hit_pos[2]) > 0.5)
		return false;

	t = length(model2world_matrix * (t * direction_model));
	hit_normal = normal_world;
	hit_surface_color = surface_color;

	return true;
}