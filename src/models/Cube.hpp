#ifndef CUBE
#define CUBE

#include "basic_model.hpp"
#include "../common.hpp"
#include "../utils/Box.hpp"

class Cube : public BasicModel {
private:
	Box box;
	vec3 surface_color;

public:
	Cube(const vec3 &position, const vec3 &size, const vec3 &surface_color, const Material &material):
		BasicModel(material, false, vec3(0.0f), vec3(0)), surface_color(surface_color), box(position, size) {}

	bool intersect(const vec3 &position, const vec3 &direction, float &t, vec3 &hit_normal, vec3 &hit_surface_color) const;
};

#endif