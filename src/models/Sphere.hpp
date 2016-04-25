#ifndef SPHERE
#define SPHERE

#include "basic_model.hpp"
#include "../common.hpp"

class Sphere : public BasicModel {
private:
	vec3 surface_color;
	vec3 center;
	float radius, radius_2;

public:
	Sphere(const vec3 &center, float radius, const vec3 &surface_color, const Material &material):
		BasicModel(material, false, vec3(0.0f), vec3(0.0)), surface_color(surface_color), center(center), radius(radius), radius_2(radius * radius) {}

	Sphere(const vec3 &emissionColor, const vec3 &center, float radius):
		BasicModel(Material(), true, emissionColor, center), surface_color(0), center(center), radius(radius), radius_2(radius * radius) {}

	bool intersect(const vec3 &position, const vec3 &direction, float &t, vec3 &hit_normal, vec3 &hit_surface_color, BasicModel const* &hit_model) const;

	bool intersectBox(const Box &box) const;

	inline vec3 getMinPs() const {
		return center - vec3(radius);
	}

	inline vec3 getMaxPs() const {
		return center + vec3(radius);
	}
};

#endif