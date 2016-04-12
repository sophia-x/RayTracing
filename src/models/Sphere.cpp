#include <cmath>

#include "Sphere.hpp"

bool Sphere::intersect(const vec4 &position, const vec4 &direction, float &t) const {
	vec4 position_model = world2model_matrix * position;
	vec4 direction_model = normalize(world2model_matrix * direction);

	vec4 position2center = -vec4(vec3(position_model), 0);
	float proj_len = dot(position2center, direction_model);

	if (proj_len <= 0)
		return false;

	float dist2ray_2 = dot(position2center, position2center) - proj_len * proj_len;
	if (dist2ray_2 >= 1)
		return false;

	float dt = sqrt(1 - dist2ray_2);
	float t1 = proj_len - dt;
	t = t1 >= 0 ? t1 : proj_len + dt;
	t = length(model2world_matrix * (vec4(t) * direction_model));

	return true;
}

vec4 Sphere::normal(const vec4 &position) const {
	return normalize(model2world_matrix * (vec4(vec3(world2model_matrix * position), 0.0)));
}

vec3 Sphere::surface_color(vec4 &position) const {
	if (isEmission)
		return emissionColor;
	return surfaceColor;
}

vec3 Sphere::emission_color() const {
	return emissionColor;
};