#ifndef MATERIAL
#define MATERIAL

#include "../common.hpp"

class Material {
private:
	float __diffuse, __specular, __specular_power, __reflection, __transparency, __refraction_radio, __absorbance;
	bool __is_light;

public:
	Material(float diffuse, float specular, float specular_power, float reflection, float transparency,
	         float refraction_radio, float absorbance, bool is_light=false): __diffuse(diffuse), __specular(specular), __specular_power(specular_power),
		__reflection(reflection), __transparency(transparency), __refraction_radio(refraction_radio), __absorbance(absorbance), __is_light(is_light) {}

	Material(bool is_light): Material(1.0f, 0, 0, 0, 0, 0, 0, is_light) {}

	inline float getDiffuse() const {
		return __diffuse;
	}

	inline float getSpecular() const {
		return __specular;
	}

	inline float getSpecularPower() const {
		return __specular_power;
	}

	inline float getReflection() const {
		return __reflection;
	}

	inline float getTransparency() const {
		return __transparency;
	}

	inline float getRefractionRadio() const {
		return __refraction_radio;
	}

	inline float getAbsorbance() const {
		return __absorbance;
	}

	inline float isLight() const {
		return __is_light;
	}
};

#endif