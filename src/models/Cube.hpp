#ifndef CUBE
#define CUBE

#include <glm/gtc/matrix_transform.hpp>

#include "basic_model.hpp"

using namespace glm;

class Cube : public BasicModel {
private:
	vec3 surface_color;

public:
	Cube(const vec4 &position, const vec3 &size, const vec3 &surface_color, float diffuse, float specular, float specular_power,
	     float reflection, float reflect_radio, float transparency, float refraction_radio, float absorbance):
		BasicModel(diffuse, specular, specular_power, reflection, reflect_radio, transparency, refraction_radio, absorbance, model2world(position, size),
		           false, vec3(0.0f), vec4(vec3(0), 1)), surface_color(surface_color) {}

	bool intersect(const vec4 &position, const vec4 &direction, float &t, vec4 &hit_normal, vec3 &hit_surface_color) const;

private:
	inline mat4 model2world(const vec4 &position, const vec3 &size) const {
		return translate(mat4(1.0f), vec3(position)) * scale(mat4(1.0f), size);
	}
};

float plane_intersect(const vec4 &position, const vec4 &direction, const vec4 &normal, float d, int idx1, int idx2);

#endif