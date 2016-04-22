#ifndef BASIC_MODEL
#define BASIC_MODEL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/random.hpp>

using namespace glm;

class BasicModel {
protected:
	static const int MAX_MODEL_NUM;

	vec3 center;
	vec3 emissionColor;
	bool emission;

	float diffuse, specular, specular_power, reflection, reflect_radio, transparency, refraction_radio, absorbance;

	unsigned long hash_code;

public:
	BasicModel(float diffuse, float specular, float specular_power, float reflection, float reflect_radio, float transparency,
	           float refraction_radio, float absorbance, bool emission, const vec3 &emissionColor, const vec3 &center):
		diffuse(diffuse), specular(specular), specular_power(specular_power), reflection(reflection), reflect_radio(reflect_radio),
		transparency(transparency), refraction_radio(refraction_radio), absorbance(absorbance), emission(emission),
		emissionColor(emissionColor), center(center), hash_code(linearRand(1, MAX_MODEL_NUM)) {}

	virtual ~BasicModel() {}

	virtual bool intersect(const vec3 &position, const vec3 &direction, float &t, vec3 &hit_normal, vec3 &hit_surface_color) const = 0;

	inline vec3 getCenter() const {
		return center;
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

	inline float getReflectRadio() const {
		return reflect_radio;
	}

	inline float getTransparency() const {
		return transparency;
	}

	inline float getRefractionRadio() const {
		return refraction_radio;
	}

	inline float getAbsorbance() const {
		return absorbance;
	}

	inline unsigned long getHashCode() const {
		return hash_code;
	}
};

#endif