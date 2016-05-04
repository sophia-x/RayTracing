#ifndef AREA_LIGHT
#define AREA_LIGHT

#include <glm/gtc/random.hpp>

#include "Light.hpp"
#include "Triangle.hpp"

#include "../common.hpp"
#include "../scenes/scene.hpp"

class AreaLight : public Light {
protected:
	Triangle __tri_a, __tri_b;
	float __width, __height;
	vec3 __position;
	vec3 __min_p;
	vec3 __a, __b, __c, __d;

public:
	AreaLight(const vec3 &position, float width, float height, const vec3 &surface_color): Light(Material(true), surface_color),
		__width(width), __height(height), __position(position), __a(position + vec3(-width / 2, 0, height / 2)),
		__b(position + vec3(width / 2, 0, height / 2)), __c(position + vec3(width / 2, 0, -height / 2)),
		__d(position + vec3(-width / 2, 0, -height / 2)),
		__tri_a(__a, __b, __c, __hash_code, __surface_color, __material),
		__tri_b(__c, __d, __a, __hash_code, __surface_color, __material),
		__min_p(position - vec3(width, 0, height)) {}

	inline float calcShade(const Scene * scene_ptr, const vec3 & hit_position, vec3 & hit2light_dir) const {
		float shade_idx = 0;
		// Monte Carlo rendering
		float dx = __width / SHADE_SIZE, dy = __height / SHADE_SIZE;
		for (size_t i = 0; i < SAMPLES; i ++) {
			int idx = i % SHADE_SIZE_2;
			int w_idx = idx / SHADE_SIZE, h_idx = idx % SHADE_SIZE;
			float w = __min_p[0] + w_idx * dx, h = __min_p[2] + h_idx * dy;

			vec3 lp(w + linearRand(0.0f, dx), __min_p[1], h + linearRand(0.0f, dy) );
			vec3 hit2light = lp - hit_position;
			float len = length(hit2light);
			hit2light_dir = hit2light / len;

			if (!scene_ptr->intersect(Ray(hit_position + EPSILON * hit2light_dir, hit2light_dir), len, __hash_code))
				shade_idx ++;
		}

		vec3 hit2light = __position - hit_position;
		float len = length(hit2light);
		hit2light_dir = hit2light / len;

		return shade_idx / SAMPLES;
	}

	inline virtual void addPrimitives(vector<const Primitive *> &primitives) const {
		primitives.push_back(&__tri_a);
		primitives.push_back(&__tri_b);
	}
};

#endif