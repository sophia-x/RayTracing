#include "AABB.hpp"

const int AABB::CUBE_NUM = 6;

bool AABB::intersect(const vec3 &position, const vec3 &direction, const vec3 &inv_direction) const {
	const vec3 &min_p = getMinPs();
	const vec3 &max_p = getMaxPs();

	float tx1 = (min_p[0] - position[0]) * inv_direction[0];
	float tx2 = (max_p[0] - position[0]) * inv_direction[0];

	float tmin = std::min(tx1, tx2);
	float tmax = std::max(tx1, tx2);

	float ty1 = (min_p[1] - position[1]) * inv_direction[1];
	float ty2 = (max_p[1] - position[1]) * inv_direction[1];

	tmin = std::max(tmin, std::min(ty1, ty2));
	tmax = std::min(tmax, std::max(ty1, ty2));

	float tz1 = (min_p[2] - position[2]) * inv_direction[2];
	float tz2 = (max_p[2] - position[2]) * inv_direction[2];

	tmin = std::max(tmin, std::min(tz1, tz2));
	tmax = std::min(tmax, std::max(tz1, tz2));

	return tmax >= tmin;
}