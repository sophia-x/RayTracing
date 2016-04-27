#ifndef LIGHT
#define LIGHT

#include "SphereModel.hpp"
#include "../common.hpp"

class Light : public SphereModel {
private:
	vec3 __emission_color;

public:
	Light(const vec3 &center, float radius, const vec3 &surface_color): SphereModel(center, radius, surface_color, Material(true)), __emission_color(surface_color) {}

	inline const vec3 &getCenter() const {
		return __sphere.getCenter();
	}

	inline const vec3 &getEmissionColor() const {
		return __emission_color;
	}
};

#endif