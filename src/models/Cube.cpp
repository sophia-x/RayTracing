#include <cmath>

#include "Cube.hpp"

using namespace std;

#define CUBE_NUM 6
#define size 0.5

const vec4 normals[CUBE_NUM] = {vec4(1, 0, 0, 0), vec4(-1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, -1, 0, 0), vec4(0, 0, 1, 0), vec4(0, 0, -1, 0)};
const int check_idx[CUBE_NUM][2] = {{1, 2}, {1, 2}, {0, 2}, {0, 2}, {0, 1}, {0, 1}};

bool Cube::intersect(const vec4 &position, const vec4 &direction, float &t, vec4 &hit_normal, vec3 &hit_surface_color) const {
	vec4 position_model = world2model_matrix * position;
	vec4 direction_model = normalize(world2model_matrix * direction);

	float min_t = numeric_limits<float>::max();
	int idx = -1;
	for (size_t i = 0; i < CUBE_NUM; i ++) {
		float tmp = plane_intersect(position_model, direction_model, normals[i], -size, check_idx[i][0], check_idx[i][1]);

		if (tmp >= 0 && tmp < min_t) {
			min_t = tmp;
			idx = i;
		}
	}
	if (idx == -1)
		return false;

	hit_normal = normalize(vec4(vec3(normal2world_matrix * normals[idx]), 0));
	hit_surface_color =  surface_color;
	t = length(model2world_matrix * (min_t * direction_model));

	return true;
}

float Cube::plane_intersect(const vec4 &position, const vec4 &direction, const vec4 &normal, float d, int idx1, int idx2) const {
	float nd = dot(normal, direction);
	if (abs(nd) <= numeric_limits<float>::epsilon())
		return -1;

	float t = -(d + dot(normal, position)) / nd;
	if (t < 0)
		return -1;

	vec4 p = position + t * direction;
	if (abs(p[idx1]) > size || abs(p[idx2]) > size)
		return -1;

	return t;
}