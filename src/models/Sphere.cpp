#include "Sphere.hpp"

bool Sphere::intersect(const vec3 &position, const vec3 &direction, const vec3 &inv_direction, float &t, vec3 &hit_normal, vec3 &hit_surface_color, BasicModel const* &hit_model) const {
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

bool Sphere::intersect(const vec3 &position, const vec3 &direction, const vec3 &inv_direction, float len) const {
	vec3 position2center = center - position;
	float proj_len = dot(position2center, direction);

	if (proj_len <= 0)
		return false;

	float center2ray_2 = dot(position2center, position2center) - proj_len * proj_len;
	if (center2ray_2 >= radius_2)
		return false;

	float dt = sqrt(radius_2 - center2ray_2);
	float t1 = proj_len - dt;
	float t = t1 >= 0 ? t1 : proj_len + dt;
	return t < len;
}

bool Sphere::intersect(const AABB &box) const {
	float d = 0;
	const vec3 &min_p = box.getMinPs();
	const vec3 &max_p = box.getMaxPs();

	for (int i = 0; i < 3; i ++) {
		if (center[i] < min_p[i]) {
			d += (center[i] - min_p[i]) * (center[i] - min_p[i]);
		} else if (center[i] > max_p[i]) {
			d += (center[i] - max_p[i]) * (center[i] - max_p[i]);
		}
	}

	return d <= radius_2;
}