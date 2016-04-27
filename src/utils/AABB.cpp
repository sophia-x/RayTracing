#include "AABB.hpp"

bool AABB::intersect(const Ray &ray, float &t_near, float &t_far) const {
	const vec3 &position = ray.getPosition();
	const vec3 &inv_direction = ray.getInvDirection();

	t_near = (__min_p[0] - position[0]) * inv_direction[0];
	t_far = (__max_p[0] - position[0]) * inv_direction[0];
	if (inv_direction[0] < 0.0f)
		swap(t_near, t_far);

	for (size_t i = 1; i < 3; i ++) {
		float t0 = (__min_p[i] - position[i]) * inv_direction[i];
		float t1 = (__max_p[i] - position[i]) * inv_direction[i];
		if (inv_direction[i] < 0.0f)
			swap(t0, t1);
		t_near = std::max(t0, t_near);
		t_far = std::min(t1, t_far);

		if (t_far < t_near)
			return false;
	}

	return true;
}