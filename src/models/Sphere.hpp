#ifndef SPHERE
#define SPHERE

#include <glm/gtc/matrix_transform.hpp>

#include "basic_model.hpp"

using namespace glm;

class Sphere : public BasicModel {
private:
	vec3 surfaceColor;

public:
	Sphere(const vec3 &surfaceColor, float transparency, float reflection)
		: BasicModel(transparency, reflection, false, vec3(0.0f), mat4(1.0f), vec4(vec3(0), 1)), surfaceColor(surfaceColor) {}

	Sphere(const vec4 &center, float radius, const vec3 &surfaceColor, float transparency, float reflection)
		: BasicModel(transparency, reflection, false, vec3(0.0f), model2world(center, radius), vec4(vec3(0), 1)), surfaceColor(surfaceColor) {}

	Sphere(const vec3 &surfaceColor, float transparency, float reflection, const vec3 &emissionColor)
		: BasicModel(transparency, reflection, true, emissionColor, mat4(1.0f), vec4(vec3(0), 1)), surfaceColor(surfaceColor) {}

	Sphere(const vec4 &center, float radius, const vec3 &surfaceColor, float transparency, float reflection, const vec3 &emissionColor)
		: BasicModel(transparency, reflection, true, emissionColor, model2world(center, radius), vec4(vec3(0), 1)), surfaceColor(surfaceColor) {}

	bool intersect(const vec4 &position, const vec4 &direction, float &t) const;

	vec4 normal(const vec4 &position) const;

	vec3 surface_color(vec4 &position) const;

	vec3 emission_color() const;

private:
	mat4 model2world(const vec4 &center, float radius) const {
		return translate(mat4(1.0f), vec3(center)) * scale(mat4(1.0f), vec3(radius));
	}
};

#endif