#include "Mesh.hpp"

bool Mesh::intersect(const vec4 &position, const vec4 &direction, float &t, vec4 &hit_normal, vec3 &hit_surface_color) const {
	vec4 position_model = world2model_matrix * position;
	vec4 direction_model = normalize(world2model_matrix * direction);

	float min_t = numeric_limits<float>::max();
	vec4 n; vec3 color;
	for (vector<Triangle>::const_iterator it = tris.begin(); it != tris.end(); ++it) {
		if (!(*it).intersect(position_model, direction_model, t, n, color))
			continue;
		if (t < min_t) {
			min_t = t;
			hit_normal = n;
			hit_surface_color = color;
		}
	}

	if (min_t == numeric_limits<float>::max())
		return false;

	t = length(model2world_matrix * (min_t * direction_model));
	hit_normal = normalize(vec4(vec3(normal2world_matrix * hit_normal), 0));
	hit_surface_color = color;
	return true;
}

bool Triangle::intersect(const vec4 &position, const vec4 &direction, float &t, vec4 &hit_normal, vec3 &hit_surface_color) const {
	vec3 e1 = vec3(a - b), e2 = vec3(a - c), s = vec3(a - position), d = vec3(direction);
	float d12 = determinant(mat3(d, e1, e2));
	if (abs(d12) < numeric_limits<float>::epsilon())
		return false;

	t = determinant(mat3(s, e1, e2));
	if (t < 0)
		return false;

	float beta = determinant(mat3(d, s, e2)) / d12, gama = determinant(mat3(d, e1, s)) / d12;
	if (beta < 0 || gama < 0 || beta > 1 || gama > 1 || beta + gama > 1)
		return false;

	t /= d12;
	hit_normal = normal_model;
	hit_surface_color = surface_color;
	return true;
}