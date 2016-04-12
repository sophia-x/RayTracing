#ifndef BASIC_MODEL
#define BASIC_MODEL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>

using namespace glm;

class BasicModel {
protected:
	mat4 model2world_matrix, world2model_matrix, normal2world_matrix;
	vec3 emissionColor;
	vec4 center;
	float transparency, reflection;
	const bool isEmission;

public:
	BasicModel(float transparency, float reflection, bool isEmission, const vec3 &emissionColor, const mat4 &model2world_matrix, const vec4 &center)
		: transparency(transparency), reflection(reflection), isEmission(isEmission), emissionColor(emissionColor)
		, model2world_matrix(model2world_matrix), world2model_matrix(inverse(model2world_matrix)), normal2world_matrix(inverseTranspose(model2world_matrix)), center(center) {}

	virtual ~BasicModel() {}

	virtual bool intersect(const vec4 &position, const vec4 &direction, float &t) const = 0;

	virtual vec4 normal(const vec4 &position) const = 0;

	virtual vec3 surface_color(vec4 &position) const = 0;

	virtual vec3 emission_color() const = 0;

	vec4 get_center() const {
		return model2world_matrix * center;
	}
};

#endif