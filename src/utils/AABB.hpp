#ifndef AABB_C
#define AABB_C

#include "../common.hpp"
#include "../cameras/Ray.hpp"

class AABB {
private:
	vec3 __position, __size, __half_size, __min_p, __max_p;

public:
	AABB() {}

	AABB(const vec3 &position, const vec3 &size): __position(position), __size(size) {
		__half_size = __size / 2.0f;
		__min_p = __position - __half_size;
		__max_p = __position + __half_size;
	}

	bool intersect(const Ray &ray, float &t_near, float &t_far) const;

	inline bool intersect(const AABB &box) const {
		vec3 b_min = box.getMinP(), b_max = box.getMaxP();

		for (int i = 0; i < 3; i ++) {
			if (__max_p[i] < b_min[i])
				return false;
			if (__min_p[i] > b_max[i])
				return false;
		}
		return true;
	}

	inline const vec3 &getMinP() const {
		return __min_p;
	}

	inline const vec3& getMaxP() const {
		return __max_p;
	}

	inline const vec3& getPos() const {
		return __position;
	}

	inline const vec3& getSize() const {
		return __size;
	}

	inline const vec3& getHalfSize() const {
		return __half_size;
	}

	inline float area() const {
		return 2 * (__size[0] * __size[1] + __size[0] * __size[2] + __size[1] * __size[2]);
	}
};

#endif