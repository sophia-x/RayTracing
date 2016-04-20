#ifndef MESH
#define MESH

#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "basic_model.hpp"

using namespace glm;
using namespace std;

class Triangle {
private:
	const vec4 &a, &b, &c;
	vec4 normal_model;
	const vec3 &surface_color;

public:
	Triangle(const vec4 &a, const vec4 &b, const vec4 &c, const vec3 &surface_color): a(a), b(b), c(c), surface_color(surface_color),
		normal_model(normalize(cross(vec3(a - b), vec3(b - c))), 0.0f) {}
	bool intersect(const vec4 &position, const vec4 &direction, float &t, vec4 &hit_normal, vec3 &hit_surface_color) const;
};

class Mesh : public BasicModel {
private:
	vector<Triangle> tris;
	vector<vec4> vertices;

public:
	Mesh(const char *file_name, const vec3 &surface_color, float diffuse, float specular, float specular_power,
	     float reflection, float reflect_radio, float transparency, float refraction_radio, float absorbance):
		BasicModel(diffuse, specular, specular_power, reflection, reflect_radio, transparency, refraction_radio, absorbance, mat4(1.0f),
		           false, vec3(0.0f), vec4(vec3(0), 1)) {
		loadObj(file_name, surface_color);
	}

	bool intersect(const vec4 &position, const vec4 &direction, float &t, vec4 &hit_normal, vec3 &hit_surface_color) const;

private:
	void loadObj(const char* file_name, const vec3 &surface_color);
};

#endif