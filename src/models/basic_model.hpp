#ifndef BASIC_MODEL
#define BASIC_MODEL

#include <glm/glm.hpp>

using namespace glm;

class BasicModel{
protected:
	mat4 model2world_matrix, world2model_matrix;
	vec3 emissionColor;
    float transparency, reflection;
    const bool isEmission;

public:
	BasicModel(float transparency, float reflection, bool isEmission, const vec3 &emissionColor, const mat4 &model2world_matrix)
				:transparency(transparency), reflection(reflection), isEmission(isEmission), emissionColor(emissionColor)
					, model2world_matrix(model2world_matrix), world2model_matrix(inverse(model2world_matrix)){}

	virtual ~BasicModel(){}

	virtual bool intersect(const vec3 &position, const vec3 &direction, float &t) const = 0;
};

#endif