#ifndef MATERIAL
#define MATERIAL

#include "Texture.hpp"
#include "../common.hpp"

class Material {
private:
	float __diffuse, __specular, __specular_power, __reflection, __transparency, __refraction_radio, __absorbance;
	bool __is_light;
	bool __has_texture;
	vec3 __color;
	const Texture *__texture;
	float __u_scale, __v_scale, __ru_scale, __rv_scale;

public:
	Material(const Texture *texture, float u_scale, float v_scale, const vec3 &color, float diffuse, float specular, float specular_power, float reflection, float transparency,
	         float refraction_radio, float absorbance, bool is_light = false): __texture(texture), __has_texture(true), __color(color), __u_scale(u_scale), __v_scale(v_scale), __ru_scale(1 / u_scale), __rv_scale(1 / v_scale), __diffuse(diffuse), __specular(specular), __specular_power(specular_power),
		__reflection(reflection), __transparency(transparency), __refraction_radio(refraction_radio), __absorbance(absorbance), __is_light(is_light) {}

	Material(const vec3 &color, float diffuse, float specular, float specular_power, float reflection, float transparency,
	         float refraction_radio, float absorbance, bool is_light = false): __color(color), __has_texture(false), __diffuse(diffuse), __specular(specular), __specular_power(specular_power),
		__reflection(reflection), __transparency(transparency), __refraction_radio(refraction_radio), __absorbance(absorbance), __is_light(is_light) {}

	Material(const vec3 &color, bool is_light): Material(color, 1.0f, 0, 0, 0, 0, 0, 0, is_light) {}

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

	inline float getUScale() const {
		return __u_scale;
	}

	inline float getVScale() const {
		return __v_scale;
	}

	inline float getRuScale() const {
		return __ru_scale;
	}

	inline float getRvScale() const {
		return __rv_scale;
	}

	inline bool hasTexture() const {
		return __has_texture;
	}

	inline vec3 getColor() const {
		return __color;
	}

	inline vec3 getColor(float u, float v) const {
		return __texture->getUvColor(u, v) * __color;
	}
};

#endif