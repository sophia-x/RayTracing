#ifndef SPHERE
#define SPHERE

#include <glm/gtc/matrix_transform.hpp>

#include "basic_model.hpp"

using namespace glm;

class Sphere : public BasicModel {
private:
	vec3 surfaceColor;

public:
	Sphere(const vec4 &center, float radius, const vec3 &surfaceColor, float diffuse, float specular, float specular_power,
	       float reflection, float reflect_radio, float transparency, float refraction_radio, float absorbance):
		BasicModel(diffuse, specular, specular_power, reflection, reflect_radio, transparency, refraction_radio, absorbance, model2world(center, radius),
		           false, vec3(0.0f), vec4(vec3(0), 1)), surfaceColor(surfaceColor) {}

	Sphere(const vec3 &emissionColor, const vec4 &center, float radius):
		BasicModel(0, 0, 0, 0, 0, 0, 0, 0, model2world(center, radius), true, emissionColor, vec4(vec3(0), 1)), surfaceColor(0) {}

	bool intersect(const vec4 &position, const vec4 &direction, float &t, vec4 &hit_normal, vec3 &hit_surface_color) const;

private:
	inline mat4 model2world(const vec4 &center, float radius) const {
		return translate(mat4(1.0f), vec3(center)) * scale(mat4(1.0f), vec3(radius));
	}
};

#endif