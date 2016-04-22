#ifndef MESH
#define MESH

#include "basic_model.hpp"
#include "../common.hpp"
#include "../utils/BoundingBox.hpp"

class Triangle {
private:
	const vec3 &a, &b, &c;
	vec3 e1, e2;
	vec3 normal;
	const vec3 &surface_color;

public:
	Triangle(const vec3 &a, const vec3 &b, const vec3 &c, const vec3 &surface_color): a(a), b(b), c(c), surface_color(surface_color),
		normal(normalize(cross(a - b, b - c))), e1(a - b), e2(a - c) {}

	bool intersect(const vec3 &position, const vec3 &direction, float &t, vec3 &hit_normal, vec3 &hit_surface_color) const;

	inline vec3 getMinPs() const {
		return glm::min(a, glm::min(b, c));
	}

	inline vec3 getMaxPs() const {
		return glm::max(a, glm::max(b, c));
	}
};

class Mesh : public BasicModel {
private:
	vector<Triangle> tris;
	vector<vec3> vertices;
	BoundingBox<Triangle> bbox;

public:
	Mesh(const char *file_name, size_t threshold, const vec3 &surface_color, float diffuse, float specular, float specular_power,
	     float reflection, float reflect_radio, float transparency, float refraction_radio, float absorbance):
		BasicModel(diffuse, specular, specular_power, reflection, reflect_radio, transparency, refraction_radio, absorbance,
		           false, vec3(0.0f), vec3(0)) {
		loadObj(file_name, surface_color);
		bbox = BoundingBox<Triangle>(tris, threshold);
	}

	bool intersect(const vec3 &position, const vec3 &direction, float &t, vec3 &hit_normal, vec3 &hit_surface_color) const;

private:
	void loadObj(const char* file_name, const vec3 &surface_color);
};

#endif