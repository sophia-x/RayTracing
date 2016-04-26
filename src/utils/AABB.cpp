#include "AABB.hpp"

const int AABB::CUBE_NUM = 6;

bool AABB::intersect(const vec3 &__position, const vec3 &direction, const vec3 &inv_direction, float &t_near, float &t_far) const {
	float tmin, tmax, tymin, tymax, tzmin, tzmax;

	if (inv_direction[0] > 0) {
		tmin = (min_p[0] - __position[0]) * inv_direction[0];
		tmax = (max_p[0] - __position[0]) * inv_direction[0];
	} else {
		tmin = (max_p[0] - __position[0]) * inv_direction[0];
		tmax = (min_p[0] - __position[0]) * inv_direction[0];
	}

	if (inv_direction[1] > 0) {
		tymin = (min_p[1] - __position[1]) * inv_direction[1];
		tymax = (max_p[1] - __position[1]) * inv_direction[1];
	} else {
		tymin = (max_p[1] - __position[1]) * inv_direction[1];
		tymax = (min_p[1] - __position[1]) * inv_direction[1];
	}

	if ((tmin > tymax) || (tymin > tmax))
		return false;
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;

	if (inv_direction[2] > 0) {
		tzmin = (min_p[2] - __position[2]) * inv_direction[2];
		tzmax = (max_p[2] - __position[2]) * inv_direction[2];
	} else {
		tzmin = (max_p[2] - __position[2]) * inv_direction[2];
		tzmax = (min_p[2] - __position[2]) * inv_direction[2];
	}

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;

	t_near = tmin;
	t_far = tmax;

	return true;
}