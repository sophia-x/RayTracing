#include <limits>

#include "Box.hpp"
#include "intersect_tool.hpp"

using namespace std;

const int Box::CUBE_NUM = 6;

bool Box::intersect(const vec3 &position, const vec3 &direction, float &t, int &idx) const {
	int min_idxs[CUBE_NUM / 2], max_idxs[CUBE_NUM / 2];
	float min_ts[CUBE_NUM / 2], max_ts[CUBE_NUM / 2];

	for (size_t i = 0; i < CUBE_NUM / 2; i ++) {
		slab_intersect(position, direction, 2 * i, 2 * i + 1, min_idxs[i], max_idxs[i], min_ts[i], max_ts[i]);
	}

	float min_t = -numeric_limits<float>::max(); int min_idx;
	for (size_t i = 0; i < CUBE_NUM / 2; i++) {
		if (min_t < min_ts[i]) {
			min_t = min_ts[i]; min_idx = i;
		}
	}

	float max_t = numeric_limits<float>::max(); int max_idx;
	for (size_t i = 0; i < CUBE_NUM / 2; i++) {
		if (max_t > max_ts[i]) {
			max_t = max_ts[i]; max_idx = i;
		}
	}

	if (max_t < 0 || min_t >= max_t) {
		return false;
	}

	if (min_t < 0) {
		t = max_t;
		idx = max_idxs[max_idx];
		return true;
	}

	t = min_t;
	idx = min_idxs[min_idx];
	return true;
}

void Box::slab_intersect(const vec3 &position, const vec3 &direction, size_t p1, size_t p2, int &min_idx, int &max_idx, float &min_t, float &max_t) const {
	min_idx = p1; max_idx = p2;
	plane_intersect(position, direction, cube_normals[min_idx], ds[min_idx], min_t);
	plane_intersect(position, direction, cube_normals[max_idx], ds[max_idx], max_t);

	if (min_t > max_t) {
		float temp = min_t; min_t = max_t; max_t = temp;
		min_idx = p2; max_idx = p1;
	}
}