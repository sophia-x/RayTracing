#ifndef TRIANGLE
#define TRIANGLE

#include "Primitive.hpp"
#include "../common.hpp"

inline bool __planeBoxOverlap(const vec3& normal, const vec3 &v, const vec3 &box_half_size);

const static vec2 dummy(0);

class Triangle: public Primitive {
private:
	const vec3 &__a, &__b, &__c;
	const vec2 &__uv_a, &__uv_b, &__uv_c;
	vec3 __e1, __e2;
	vec3 __normal;
	float __u, __v;

public:
	Triangle(const vec3 &a, const vec3 &b, const vec3 &c, unsigned long hash_code, const Material &material):
		Primitive(hash_code, material), __a(a), __b(b), __c(c), __e1(b - a), __e2(c - a), __normal(normalize(cross(a - b, b - c))), __uv_a(dummy), __uv_b(dummy), __uv_c(dummy) {}

	Triangle(const vec3 &a, const vec3 &b, const vec3 &c, const vec2 &uv_a, const vec2 &uv_b, const vec2 &uv_c, unsigned long hash_code, const Material &material):
		Primitive(hash_code, material), __a(a), __b(b), __c(c), __e1(b - a), __e2(c - a), __normal(normalize(cross(a - b, b - c))), __uv_a(uv_a), __uv_b(uv_b), __uv_c(uv_c) {}

	bool intersect(const Ray &ray, float &t);

	bool intersect_seg(const Ray &ray, float len) const;

	bool intersect(const AABB &box) const;

	inline vec3 getNormal(const vec3 &hit_position) const {
		return __normal;
	}

	inline vec3 getColor(const vec3 &hit_position) const {
		if (!__material.hasTexture())
			return __material.getColor();

		float u = (__uv_a[0] + __u * (__uv_b[0] - __uv_a[0]) + __v * (__uv_c[0] - __uv_a[0])) * __material.getRuScale();
		float v = (__uv_a[1] + __u * (__uv_b[1] - __uv_a[1]) + __v * (__uv_c[1] - __uv_a[1])) * __material.getRvScale();
		return __material.getColor( u, v );
	}

	inline vec3 getMinP() const {
		return glm::min(__a, glm::min(__b, __c));
	}

	inline vec3 getMaxP() const {
		return glm::max(__a, glm::max(__b, __c));
	}
};

#endif