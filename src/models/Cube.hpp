#ifndef CUBE
#define CUBE

#include "basic_model.hpp"
#include "../common.hpp"
#include "../utils/Box.hpp"

class Cube : public BasicModel {
private:
	Box box;
	vec3 surface_color;
	vec3 position;
	vec3 size;

public:
	Cube(const vec3 &position, const vec3 &size, const vec3 &surface_color, const Material &material):
		BasicModel(material, false, vec3(0.0f), vec3(0)), position(position), size(size), surface_color(surface_color), box(position, size) {}

	bool intersect(const vec3 &position, const vec3 &direction, float &t, vec3 &hit_normal, vec3 &hit_surface_color, BasicModel const* &hit_model) const;

	inline vec3 getMinPs() const {
		return box.getMinPs();
	}

	inline vec3 getMaxPs() const {
		return box.getMaxPs();
	}
};

#endif