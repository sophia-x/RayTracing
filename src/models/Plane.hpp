#ifndef PLANE
#define PLANE

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "basic_model.hpp"

using namespace glm;

class Plane : public BasicModel {
private:
	vec3 surface_color;
	vec3 normal;
	float d;

public:
	Plane(const vec3 &__normal, float d, const vec3 &surface_color, float diffuse, float specular, float specular_power,
	      float reflection, float reflect_radio, float transparency, float refraction_radio, float absorbance):
		BasicModel(diffuse, specular, specular_power, reflection, reflect_radio, transparency, refraction_radio, absorbance,
		           false, vec3(0.0f), vec3(0)), surface_color(surface_color), normal(__normal), d(d) {
		float len = length(normal);
		normal /= len;
		d /= len;
	}

	bool intersect(const vec3 &position, const vec3 &direction, float &t, vec3 &hit_normal, vec3 &hit_surface_color) const;
};

#endif