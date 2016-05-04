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
	const vec3 &__surface_color;

public:	
	Primitive(unsigned long hash_code, const Material &material, const vec3 &surface_color): __hash_code(hash_code), __material(material), __surface_color(surface_color) {}

	virtual ~Primitive() {}

	virtual bool intersect(const Ray &ray, float &t, vec3 &hit_normal, vec3 &hit_surface_color) const = 0;

	virtual bool intersect(const Ray &ray, float len) const = 0;

	virtual bool intersect(const AABB &box) const = 0;

	virtual vec3 getMinP() const = 0;

	virtual vec3 getMaxP() const = 0;

	inline const Material &getMaterial() const {
		return __material;
	}

	inline unsigned long getHashCode() const {
		return __hash_code;
	}

	inline const vec3& getSurfaceColor() const {
		return __surface_color;
	}
};

#endif