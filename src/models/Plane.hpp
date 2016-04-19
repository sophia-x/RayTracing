#ifndef PLANE
#define PLANE

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "basic_model.hpp"

using namespace glm;

class Plane : public BasicModel {
private:
	vec3 surface_color;

public:
	Plane(const vec4 &normal, float loc, float side, const vec3 &surface_color, float diffuse, float specular, float specular_power,
	      float reflection, float reflect_radio, float transparency, float refraction_radio, float absorbance):
		BasicModel(diffuse, specular, specular_power, reflection, reflect_radio, transparency, refraction_radio, absorbance, model2world(normal, loc, side),
		           false, vec3(0.0f), vec4(vec3(0), 1)), surface_color(surface_color) {}

	bool intersect(const vec4 &position, const vec4 &direction, float &t, vec4 &hit_normal, vec3 &hit_surface_color) const;

private:
	inline mat4 model2world(const vec4 &normal, float loc, float side) const {
		vec3 n(0, 1, 0);
		vec3 axis = cross(n, vec3(normal));
		if (dot(axis, axis) <= 0.5)
			return translate(mat4(1.0f), loc * vec3(normal)) * scale(mat4(1.0), vec3(side));

		float angle = - orientedAngle(n, vec3(normal), axis);
		return translate(mat4(1.0f), loc * vec3(normal)) * rotate(mat4(1.0f), angle, axis) * scale(mat4(1.0), vec3(side));
	}
};

#endif