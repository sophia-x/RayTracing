#include "model_util.hpp"

Mesh *Plane(const vec3 &a, const vec3 &b, const vec3 &c, const vec3 &d, const Material &material) {
	vector<vec3> vertices = {a, b, c, d};
	vector<int> tri_idx = {0, 1, 2, 2, 3, 0};

	return new Mesh(vertices, tri_idx, material);
}

Mesh *Plane(const vec3 &a, const vec3 &b, const vec3 &c, const vec3 &d, const vec2 uv_a, const vec2 uv_b, const vec2 uv_c, const vec2 uv_d, const Material &material) {
	vector<vec3> vertices = {a, b, c, d};
	vector<vec2> uvs = {uv_a, uv_b, uv_c, uv_d};
	vector<int> tri_idx = {0, 1, 2, 2, 3, 0};

	return new Mesh(vertices, uvs, tri_idx, material);
}

Mesh *Plane(const vec2 uv_a, const vec2 uv_b, const vec2 uv_c, const vec2 uv_d, const Material &material) {
	return Plane(vec3(-1, 0, -1), vec3(-1, 0, 1), vec3(1, 0, 1), vec3(1, 0, -1), uv_a, uv_b, uv_c, uv_d, material);
}

Mesh *Plane(const Material &material) {
	return Plane(vec3(-1, 0, -1), vec3(-1, 0, 1), vec3(1, 0, 1), vec3(1, 0, -1), material);
}

Mesh *Cube(const vec3 &pos, const vec3 &size, const Material &material) {
	vec3 half_size = size / 2.0f;

	vec3 a(pos[0] - half_size[0], pos[1] + half_size[1], pos[2] - half_size[2]);
	vec3 b(pos[0] + half_size[0], pos[1] + half_size[1], pos[2] - half_size[2]);
	vec3 c(pos[0] + half_size[0], pos[1] + half_size[1], pos[2] + half_size[2]);
	vec3 d(pos[0] - half_size[0], pos[1] + half_size[1], pos[2] + half_size[2]);

	vec3 e(pos[0] - half_size[0], pos[1] - half_size[1], pos[2] - half_size[2]);
	vec3 f(pos[0] + half_size[0], pos[1] - half_size[1], pos[2] - half_size[2]);
	vec3 g(pos[0] + half_size[0], pos[1] - half_size[1], pos[2] + half_size[2]);
	vec3 h(pos[0] - half_size[0], pos[1] - half_size[1], pos[2] + half_size[2]);

	vector<vec3> vertices = {a, b, c, d, e, f, g, h};
	vector<int> tri_idx = {	0, 3, 2, 2, 1, 0,
	                        0, 1, 5, 5, 4, 0,
	                        4, 5, 6, 6, 7, 4,
	                        2, 3, 7, 7, 6, 2,
	                        1, 2, 6, 6, 5, 1,
	                        0, 4, 7, 7, 3, 0
	                      };

	return new Mesh(vertices, tri_idx, material);
}

Mesh *Cube(const Material &material) {
	return Cube(vec3(0), vec3(2), material);
}