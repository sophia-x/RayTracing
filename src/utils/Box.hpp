#ifndef BOX
#define BOX

#include <glm/glm.hpp>
#include <vector>

using namespace glm;
using namespace std;

static const vec4 CUBE_NORMALS[] = {vec4(1, 0, 0, 0), vec4(-1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, -1, 0, 0), vec4(0, 0, 1, 0), vec4(0, 0, -1, 0)};

class Box {
private:
	static const int CUBE_NUM;
	static const int CHECK_IDX[][2];

	vector<vec4> borders;
	vector<float> ds;

public:
	Box() {}

	Box(const vec3 &position, const vec3 &size): borders(CUBE_NUM), ds(CUBE_NUM) {
		vec3 min_border = position - size / 2.0f;
		vec3 max_border = position + size / 2.0f;

		for (size_t i = 0; i < CUBE_NUM; i ++) {
			borders[i] = vec4(min_border[CHECK_IDX[i][0]], max_border[CHECK_IDX[i][0]], min_border[CHECK_IDX[i][1]], max_border[CHECK_IDX[i][1]]);
			if (i % 2 == 0)
				ds[i] = -dot(max_border, vec3(CUBE_NORMALS[i]));
			else
				ds[i] = -dot(min_border, vec3(CUBE_NORMALS[i]));
		}
	}

	bool intersect(const vec4 &position, const vec4 &direction, float &t, int &idx) const;

	inline const vec4 &getNormal(int i) const {
		return CUBE_NORMALS[i];
	}
};

#endif