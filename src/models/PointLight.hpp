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
	PointLight(const vec3 &emission_color): Light(Material(emission_color, true), emission_color),
		__sphere(__hash_code, __material) {}

	PointLight(const vec3 &center, float radius, const vec3 &emission_color): Light(Material(emission_color, true), emission_color),
		__sphere(center, radius, __hash_code, __material) {}

	inline vec3 calcShade(const Scene *scene_ptr, const vec3 &hit_position, float &shade_idx) const {
		vec3 hit2light = __sphere.getCenter() - hit_position;
		float len = length(hit2light);
		vec3 hit2light_dir = hit2light / len;
		shade_idx = 1;

		if (scene_ptr->intersect(Ray(hit_position + EPSILON * hit2light_dir, hit2light_dir), len, __hash_code))
			shade_idx = 0;

		return hit2light_dir;
	}

	inline void transform(const mat4 &transform_matrix) {
		__sphere.transform(transform_matrix);
	}

	inline virtual void addPrimitives(vector<Primitive *> &primitives) {
		primitives.push_back(&__sphere);
	}
};

#endif