#ifndef SPHERE
#define SPHERE

#include <glm/gtc/matrix_transform.hpp>

#include "basic_model.hpp"

using namespace glm;

class Sphere : public BasicModel{
private:
	vec3 surfaceColor;

public:
	Sphere(const vec3 &surfaceColor, float transparency, float reflection)
		:BasicModel(transparency, reflection, false, vec3(0.0f), mat4(1.0f)), surfaceColor(surfaceColor){}

	Sphere(const vec3 &center, float radius, const vec3 &surfaceColor, float transparency, float reflection)
		:BasicModel(transparency, reflection, false, vec3(0.0f), model2world(center, radius)), surfaceColor(surfaceColor){}

	Sphere(const vec3 &surfaceColor, float transparency, float reflection, const vec3 &emissionColor)
		:BasicModel(transparency, reflection, true, emissionColor, mat4(1.0f)), surfaceColor(surfaceColor){}

	Sphere(const vec3 &center, float radius, const vec3 &surfaceColor, float transparency, float reflection, const vec3 &emissionColor)
		:BasicModel(transparency, reflection, true, emissionColor, model2world(center, radius)), surfaceColor(surfaceColor){}

	bool intersect(const vec3 &position, const vec3 &direction, float &t) const;

private:
	mat4 model2world(const vec3 &center, float radius) const{
		return translate(scale(mat4(1.0f), vec3(radius)), center);
	}
};

#endif