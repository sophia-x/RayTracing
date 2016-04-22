#include "Sphere.hpp"

bool Sphere::intersect(const vec3 &position, const vec3 &direction, float &t, vec3 &hit_normal, vec3 &hit_surface_color, BasicModel const* &hit_model) const {
	vec3 position2center = center - position;
	float proj_len = dot(position2center, direction);

	if (proj_len <= 0)
		return false;

	float center2ray_2 = dot(position2center, position2center) - proj_len * proj_len;
	if (center2ray_2 >= radius_2)
		return false;

	float dt = sqrt(radius_2 - center2ray_2);
	float t1 = proj_len - dt;
	t = t1 >= 0 ? t1 : proj_len + dt;

	hit_normal = (position + t * direction - center) / radius;
	hit_surface_color =  emission ? emissionColor : surface_color;
	hit_model = this;

	return true;
}