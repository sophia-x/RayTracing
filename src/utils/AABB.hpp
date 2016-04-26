#ifndef AABB_C
#define AABB_C

#include "../common.hpp"

const static vec3 CUBE_NORMALS[] = {vec3(1, 0, 0), vec3(-1, 0, 0), vec3(0, 1, 0), vec3(0, -1, 0), vec3(0, 0, 1), vec3(0, 0, -1)};

class AABB {
private:
	static const int CUBE_NUM;
	vec3 position, size, min_p, max_p;

public:
	AABB() {}

	AABB(const vec3 &position, const vec3 &__size): position(position), size(__size) {
		for (int i = 0; i < 3; i ++) {
			if (size[i] < EPSILON)
				size[i] += EPSILON;
		}

		vec3 half_size = size / 2.0f;
		min_p = position - half_size;
		max_p = position + half_size;
	}

	bool intersect(const vec3 &position, const vec3 &direction, const vec3 &inv_direction, float &t_near, float &t_far) const;

	inline bool intersect(const AABB &box) const {
		vec3 a_min = getMinPs(), b_min = box.getMinPs(), a_max = getMaxPs(), b_max = box.getMaxPs();

		for (int i = 0; i < 3; i ++) {
			if (a_max[i] < b_min[i])
				return false;
			if (a_min[i] > b_max[i])
				return false;
		}
		return true;
	}

	inline const vec3 &getNormal(int i) const {
		return CUBE_NORMALS[i];
	}

	inline const vec3 &getMinPs() const {
		return min_p;
	}

	inline const vec3& getMaxPs() const {
		return max_p;
	}

	inline const vec3& getPos() const {
		return position;
	}

	inline const vec3& getSize() const {
		return size;
	}

	inline float area() const {
		return 2 * (size[0] * size[1] + size[0] * size[2] + size[1] * size[2]);
	}
};

#endif