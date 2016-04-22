#ifndef CUBE
#define CUBE

#include <glm/gtc/matrix_transform.hpp>

#include "basic_model.hpp"
#include "../utils/Box.hpp"

using namespace glm;

class Cube : public BasicModel {
private:
	Box box;
	vec3 surface_color;

public:
	Cube(const vec3 &position, const vec3 &size, const vec3 &surface_color, float diffuse, float specular, float specular_power,
	     float reflection, float reflect_radio, float transparency, float refraction_radio, float absorbance):
		BasicModel(diffuse, specular, specular_power, reflection, reflect_radio, transparency, refraction_radio, absorbance,
		           false, vec3(0.0f), vec3(0)), surface_color(surface_color), box(position, size) {}

	bool intersect(const vec3 &position, const vec3 &direction, float &t, vec3 &hit_normal, vec3 &hit_surface_color) const;
};

#endif