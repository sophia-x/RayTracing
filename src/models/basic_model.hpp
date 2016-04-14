#ifndef BASIC_MODEL
#define BASIC_MODEL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>

using namespace glm;

class BasicModel {
protected:
	mat4 model2world_matrix, world2model_matrix, normal2world_matrix;
	vec4 center;

	vec3 emissionColor;
	bool emission;

	float diffuse, specular, specular_power, reflection, reflect_radio, transparency, refraction_radio;

public:
	BasicModel(float diffuse, float specular, float specular_power, float reflection, float reflect_radio, float transparency,
	           float refraction_radio,   const mat4 &model2world_matrix, bool emission, const vec3 &emissionColor, const vec4 &center):
		diffuse(diffuse), specular(specular), specular_power(specular_power), reflection(reflection), reflect_radio(reflect_radio),
		transparency(transparency), refraction_radio(refraction_radio), model2world_matrix(model2world_matrix),
		world2model_matrix(inverse(model2world_matrix)), normal2world_matrix(inverseTranspose(model2world_matrix)), emission(emission),
		emissionColor(emissionColor), center(center) {}

	virtual ~BasicModel() {}

	virtual bool intersect(const vec4 &position, const vec4 &direction, float &t, vec4 &hit_normal, vec3 &hit_surface_color) const = 0;

	inline vec4 getCenter() const {
		return model2world_matrix * center;
	}

	inline vec3 getEmissionColor() const {
		return emissionColor;
	};

	inline bool isLight() const {
		return emission;
	}

	inline float getDiffuse() const {
		return diffuse;
	}

	inline float getSpecular() const {
		return specular;
	}

	inline float getSpecularPower() const {
		return specular_power;
	}

	inline float getReflection() const {
		return reflection;
	}

	inline float getReflectRadio() {
		return reflect_radio;
	}

	inline float getTransparency() const {
		return transparency;
	}

	inline float getRefractionRadio() {
		return refraction_radio;
	}
};

#endif