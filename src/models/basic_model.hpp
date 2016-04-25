#ifndef BASIC_MODEL
#define BASIC_MODEL

#include <glm/gtc/random.hpp>

#include "Material.hpp"
#include "../common.hpp"
#include "../utils/AABB.hpp"

class BasicModel {
protected:
	static const int MAX_MODEL_NUM;

	vec3 center;
	vec3 emissionColor;
	bool emission;

	Material material;

	unsigned long hash_code;

public:
	BasicModel(const Material &material, bool emission, const vec3 &emissionColor, const vec3 &center): material(material), emission(emission),
		emissionColor(emissionColor), center(center), hash_code(linearRand(1, MAX_MODEL_NUM)) {}

	virtual ~BasicModel() {}

	virtual bool intersect(const vec3 &position, const vec3 &direction, const vec3 &inv_direction, float &t, vec3 &hit_normal, vec3 &hit_surface_color, BasicModel const* &hit_model) const = 0;

	virtual bool intersect(const AABB &box) const = 0;

	virtual inline vec3 getMinPs() const {
		return vec3(0);
	}

	virtual inline vec3 getMaxPs() const {
		return vec3(0);
	}

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
		return material.diffuse;
	}

	inline float getSpecular() const {
		return material.specular;
	}

	inline float getSpecularPower() const {
		return material.specular_power;
	}

	inline float getReflection() const {
		return material.reflection;
	}

	inline float getTransparency() const {
		return material.transparency;
	}

	inline float getRefractionRadio() const {
		return material.refraction_radio;
	}

	inline float getAbsorbance() const {
		return material.absorbance;
	}

	inline unsigned long getHashCode() const {
		return hash_code;
	}
};

#endif