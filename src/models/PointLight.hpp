#ifndef POINT_LIGHT
#define POINT_LIGHT

#include "Light.hpp"
#include "Sphere.hpp"

#include "../common.hpp"
#include "../scenes/scene.hpp"

class PointLight : public Light {
protected:
	Sphere __sphere;

public:
	PointLight(const vec3 &center, float radius, const vec3 &surface_color): Light(Material(true), surface_color),
		__sphere(center, radius, __hash_code, __surface_color, __material) {}

	inline float calcShade(const Scene *scene_ptr, const vec3 &hit_position, vec3 &hit2light_dir) const {
		vec3 hit2light = __sphere.getCenter() - hit_position;
		float len = length(hit2light);
		hit2light_dir = hit2light / len;

		if (scene_ptr->intersect(Ray(hit_position + EPSILON * hit2light_dir, hit2light_dir), len, __hash_code))
			return 0.0f;
		return 1.0f;
	}

	inline virtual void addPrimitives(vector<const Primitive *> &primitives) const {
		primitives.push_back(&__sphere);
	}
};

#endif