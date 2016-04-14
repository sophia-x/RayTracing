#include <cmath>

#include "Sphere.hpp"


bool Sphere::intersect(const vec4 &position, const vec4 &direction, float &t, vec4 &hit_normal, vec3 &hit_surface_color) const {
	vec4 position_model = world2model_matrix * position;
	vec4 direction_model = normalize(world2model_matrix * direction);

	vec4 position2center = -vec4(vec3(position_model), 0);
	float proj_len = dot(position2center, direction_model);

	if (proj_len <= 0)
		return false;

	float center2ray_2 = dot(position2center, position2center) - proj_len * proj_len;
	if (center2ray_2 >= 1)
		return false;

	float dt = sqrt(1 - center2ray_2);
	float t1 = proj_len - dt;
	t = t1 >= 0 ? t1 : proj_len + dt;

	hit_normal = normalize(model2world_matrix * vec4(vec3(position_model + t * direction_model), 0.0f));
	hit_surface_color =  emission ? emissionColor : surfaceColor;
	t = length(model2world_matrix * (t * direction_model));

	return true;
}