#ifndef BOX
#define BOX

#include "../common.hpp"

const static vec3 CUBE_NORMALS[] = {vec3(1, 0, 0), vec3(-1, 0, 0), vec3(0, 1, 0), vec3(0, -1, 0), vec3(0, 0, 1), vec3(0, 0, -1)};

class Box {
private:
	static const int CUBE_NUM;

	vector<vec3> cube_normals;
	vector<float> ds;

	vec3 position, size;

public:
	Box() {}

	Box(const vec3 &position, const vec3 &size): position(position), size(size), ds(CUBE_NUM), cube_normals(CUBE_NUM) {
		vec3 min_border = position - size / 2.0f;
		vec3 max_border = position + size / 2.0f;

		for (size_t i = 0; i < CUBE_NUM; i ++) {
			cube_normals[i] = CUBE_NORMALS[i];
			if (i % 2 == 0)
				ds[i] = -dot(max_border, cube_normals[i]);
			else
				ds[i] = -dot(min_border, cube_normals[i]);
		}
	}

	bool intersect(const vec3 &position, const vec3 &direction, float &t, int &idx) const;

	inline bool intersectBox(const Box &box) const {
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
		return cube_normals[i];
	}

	inline vec3 getMinPs() const {
		return position - size / 2.0f;
	}

	inline vec3 getMaxPs() const {
		return position + size / 2.0f;
	}

	inline vec3 getPos() const {
		return position;
	}

	inline vec3 getSize() const {
		return size;
	}

	inline float area() const {
		return 2 * (size[0] * size[1] + size[0] * size[2] + size[1] * size[2]);
	}

private:
	void slab_intersect(const vec3 &position, const vec3 &direction, size_t p1, size_t p2, int &min_idx, int &max_idx, float &min_t, float &max_t) const;
};

#endif