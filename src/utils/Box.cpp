#include <limits>

#include "Box.hpp"
#include "intersect_tool.hpp"

using namespace std;

const int Box::CUBE_NUM = 6;
const int Box::CHECK_IDX[][2] = {{1, 2}, {1, 2}, {0, 2}, {0, 2}, {0, 1}, {0, 1}};

bool Box::intersect(const vec4 &position, const vec4 &direction, float &t, int &idx) const {
	float min_t = numeric_limits<float>::max();
	idx = -1;

	for (size_t i = 0; i < CUBE_NUM; i ++) {
		if ( !plane_intersect(position, direction, CUBE_NORMALS[i], ds[i], borders[i], CHECK_IDX[i][0], CHECK_IDX[i][1], t))
			continue;

		if (t < min_t) {
			min_t = t;
			idx = i;
		}
	}
	if (idx == -1)
		return false;

	t = min_t;
	return true;
}