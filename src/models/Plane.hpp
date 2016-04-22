#ifndef PLANE
#define PLANE

#include "basic_model.hpp"
#include "../common.hpp"

class Plane : public BasicModel {
private:
	vec3 surface_color;
	vec3 normal;
	float d;

public:
	Plane(const vec3 &__normal, float d, const vec3 &surface_color, const Material &material):
		BasicModel(material, false, vec3(0.0f), vec3(0)), surface_color(surface_color), normal(__normal), d(d) {
		float len = length(normal);
		normal /= len;
		d /= len;
	}

	bool intersect(const vec3 &position, const vec3 &direction, float &t, vec3 &hit_normal, vec3 &hit_surface_color) const;
};

#endif