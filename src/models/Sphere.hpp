#ifndef SPHERE
#define SPHERE

#include "Primitive.hpp"
#include "../common.hpp"

class Sphere : public Primitive {
private:
	vec3 __center;
	float __radius, __radius_2;
	vec3 __vn, __ve, __vc;

public:
	Sphere(const vec3 &center, float radius, unsigned long hash_code, const Material &material):
		Primitive(hash_code, material), __center(center), __radius(radius), __radius_2(radius * radius) {

		__vn = vec3( 0, 1, 0 );
		__ve = vec3( 1, 0, 0 );
		__vc = cross(__vn, __ve);
	}

	bool intersect(const Ray &ray, float &t);

	bool intersect_seg(const Ray &ray, float len) const;

	bool intersect(const AABB &box) const;

	inline vec3 getNormal(const vec3 &hit_position) const {
		return (hit_position - __center) / __radius;
	}

	inline vec3 getColor(const vec3 &hit_position) const {
		if (!__material.hasTexture())
			return __material.getColor();

		vec3 normal = (hit_position - __center) / __radius;
		float phi = acosf(-dot(normal, __vn));
		float u, v = phi * __material.getRvScale() * (1.0f / PI);
		float theta = (acosf( dot( __ve, normal ) / sinf( phi ))) * (2.0f / PI);
		if (dot( __vc, normal ) >= 0) {
			u = (1.0f - theta) * __material.getRuScale();
		} else {
			u = theta * __material.getRuScale();
		}

		return __material.getColor(u, v);
	}

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