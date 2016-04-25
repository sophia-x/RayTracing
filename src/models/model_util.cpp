#include "model_util.hpp"

Mesh *Plane(const vec3 &a, const vec3 &b, const vec3 &c, const vec3 &d, const vec3 &surface_color, const Material &material) {
	vector<vec3> vertices = {a, b, c, d};
	vector<int> tri_idx = {0, 1, 2, 2, 3, 0};

	return new Mesh(vertices, tri_idx, surface_color, material);
}

Mesh *Plane(const vec3 &surface_color, const Material &material) {
	return Plane(vec3(-1, 0, -1), vec3(-1, 0, 1), vec3(1, 0, 1), vec3(1, 0, -1), surface_color, material);
}