#ifndef PLANE
#define PLANE

#include "basic_model.hpp"
#include "../common.hpp"

class Plane : public BasicModel {
private:
	vec3 surface_color;
	vec3 normal;
	float d;

	static const float MAX;

public:
	Plane(const vec3 &__normal, float d, const vec3 &surface_color, const Material &material):
		BasicModel(material, false, vec3(0.0f), vec3(0)), surface_color(surface_color), normal(__normal), d(d) {
		float len = length(normal);
		normal /= len;
		d /= len;
	}

	bool intersect(const vec3 &position, const vec3 &direction, float &t, vec3 &hit_normal, vec3 &hit_surface_color, BasicModel const* &hit_model) const;

	inline bool intersectBox(const Box &box) const {
		vec3 min_ps = getMinPs();
		vec3 max_ps = getMaxPs();
		Box b ((max_ps + min_ps) / 2.0f, max_ps - min_ps);
		return b.intersectBox(box);
	}

	inline vec3 getMinPs() const {
		return vec3(-MAX);
	}

	inline vec3 getMaxPs() const {
		return vec3(MAX);
	}
};

#endif