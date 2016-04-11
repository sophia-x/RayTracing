#include <cmath>

#include "Sphere.hpp"

bool Sphere::intersect(const vec3 &position, const vec3 &direction, float &t) const{
	vec3 position_model(world2model_matrix*vec4(position, 1.0f));
	vec3 direction_model(normalize(world2model_matrix*vec4(direction, 0.0f)));

	float proj_len = dot(-position_model, direction_model);

	if(proj_len <= 0)
		return false;

	float dist2ray_2 = dot(position_model, position_model) - proj_len*proj_len;
	if(dist2ray_2 >= 1)
		return false;

	float dt = sqrt(1 - dist2ray_2);
	float t1 = proj_len - dt;	
	t = t1 >= 0 ? t1 : proj_len+dt;
	return true;
}