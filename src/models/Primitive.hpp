#ifndef PRIMITIVE
#define PRIMITIVE

#include "Material.hpp"

#include "../common.hpp"
#include "../cameras/Ray.hpp"
#include "../utils/AABB.hpp"

class Primitive {
protected:
	unsigned long __hash_code;
	const Material &__material;

public:
	Primitive(unsigned long hash_code, const Material &material): __hash_code(hash_code), __material(material) {}

	virtual ~Primitive() {}

	virtual bool intersect(const Ray &ray, float &t) = 0;

	virtual bool intersect_seg(const Ray &ray, float len) const = 0;

	virtual bool intersect(const AABB &box) const = 0;

	virtual void getColorNormal(const vec3 &hit_position, vec3 &color, vec3 &normal) const = 0;

	virtual vec3 getMinP() const = 0;

	virtual vec3 getMaxP() const = 0;

	inline const Material &getMaterial() const {
		return __material;
	}

	inline unsigned long getHashCode() const {
		return __hash_code;
	}
};

#endif