#ifndef TRIANGLE
#define TRIANGLE

#include "Primitive.hpp"
#include "../common.hpp"

inline bool __planeBoxOverlap(const vec3& normal, const vec3 &v, const vec3 &box_half_size);

class Triangle: public Primitive {
private:
	const vec3 &__a, &__b, &__c;
	vec3 __e1, __e2;
	vec3 __normal;

public:
	Triangle(const vec3 &a, const vec3 &b, const vec3 &c, unsigned long hash_code, const vec3 &surface_color, const Material &material):
		Primitive(hash_code, material, surface_color), __a(a), __b(b), __c(c), __e1(b - a), __e2(c - a), __normal(normalize(cross(a - b, b - c))) {}

	bool intersect(const Ray &ray, float &t, vec3 &hit_normal, vec3 &hit_surface_color) const;

	bool intersect(const Ray &ray, float len) const;

	bool intersect(const AABB &box) const;

	inline vec3 getMinP() const {
		return glm::min(__a, glm::min(__b, __c));
	}

	inline vec3 getMaxP() const {
		return glm::max(__a, glm::max(__b, __c));
	}
};

#endif