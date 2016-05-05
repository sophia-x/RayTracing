#include "Sphere.hpp"

#define INTERSECT()																	\
const vec3 &position = ray.getPosition();											\
const vec3 &direction = ray.getDirection();											\
vec3 position2center = __center - position;											\
float proj_len = dot(position2center, direction);									\
if (proj_len <= 0)																	\
	return false;																	\
float center2ray_2 = dot(position2center, position2center) - proj_len * proj_len;	\
if (center2ray_2 >= __radius_2)														\
	return false;																	\
float dt = sqrt(__radius_2 - center2ray_2);											\
float t1 = proj_len - dt;															\
t = t1 >= 0 ? t1 : proj_len + dt;

bool Sphere::intersect(const Ray &ray, float &t) {
	INTERSECT()
	return true;
}

bool Sphere::intersect_seg(const Ray &ray, float len) const {
	float t;
	INTERSECT()

	return t < len;
}

bool Sphere::intersect(const AABB &box) const {
	float d = 0;
	const vec3 &min_p = box.getMinP();
	const vec3 &max_p = box.getMaxP();

	for (int i = 0; i < 3; i ++) {
		if (__center[i] < min_p[i]) {
			d += (__center[i] - min_p[i]) * (__center[i] - min_p[i]);
		} else if (__center[i] > max_p[i]) {
			d += (__center[i] - max_p[i]) * (__center[i] - max_p[i]);
		}
	}

	return d <= __radius_2;
}