#ifndef SPHERE
#define SPHERE

#include "Primitive.hpp"
#include "../common.hpp"

class Sphere : public Primitive {
private:
	vec3 __center;
	float __radius, __radius_2;

public:
	Sphere(const vec3 &center, float radius, unsigned long hash_code, const vec3 &surface_color, const Material &material):
		Primitive(hash_code, material, surface_color), __center(center), __radius(radius), __radius_2(radius * radius) {}

	bool intersect(const Ray &ray, float &t, vec3 &hit_normal, vec3 &hit_surface_color) const;

	bool intersect(const Ray &ray, float len) const;

	bool intersect(const AABB &box) const;

	inline vec3 getMinP() const {
		return __center - vec3(__radius);
	}

	inline vec3 getMaxP() const {
		return __center + vec3(__radius);
	}

	inline const vec3 &getCenter() const {
		return __center;
	}
};

#endif