#ifndef AREA_LIGHT
#define AREA_LIGHT

#include <glm/gtc/random.hpp>

#include "Light.hpp"
#include "Triangle.hpp"

#include "../common.hpp"
#include "../scenes/scene.hpp"

// Aera Light
const static float SAMPLES = 128;
const static size_t SHADE_SIZE = 4;
const static size_t SHADE_SIZE_2 = SHADE_SIZE * SHADE_SIZE;

class AreaLight : public Light {
protected:
	Triangle __tri_a, __tri_b;
	vec3 __dx, __dy;
	vec3 __a, __b, __c, __d;

public:
	AreaLight(const vec3 &emission_color): AreaLight(vec3(0), 2, 2, emission_color) {}

	AreaLight(const vec3 &position, float width, float height, const vec3 &emission_color): Light(Material(emission_color, true), emission_color),
		__a(position + vec3(-width / 2, 0, height / 2)), __b(position + vec3(width / 2, 0, height / 2)),
		__c(position + vec3(width / 2, 0, -height / 2)), __d(position + vec3(-width / 2, 0, -height / 2)),
		__tri_a(__a, __b, __c, __hash_code, __material), __tri_b(__c, __d, __a, __hash_code, __material) {
		__dx = (__c - __d) / float(SHADE_SIZE);
		__dy = (__a - __d) / float(SHADE_SIZE);
	}

	void transform(const mat4 &transform_matrix) {
		__a = vec3(transform_matrix * vec4(-1, 0, 1, 1));
		__b = vec3(transform_matrix * vec4(1, 0, 1, 1));
		__c = vec3(transform_matrix * vec4(1, 0, -1, 1));
		__d = vec3(transform_matrix * vec4(-1, 0, -1, 1));

		__tri_a.update();
		__tri_b.update();

		__dx = (__c - __d) / float(SHADE_SIZE);
		__dy = (__a - __d) / float(SHADE_SIZE);
	}

	inline vec3 calcShade(const Scene *scene_ptr, const vec3 &hit_position, float &shade_idx) const {
		shade_idx = 0;

		// Monte Carlo rendering
		vec3 total_dir(0);
		for (size_t i = 0; i < SAMPLES; i ++) {
			int idx = i % SHADE_SIZE_2;
			int w_idx = idx / SHADE_SIZE, h_idx = idx % SHADE_SIZE;

			vec3 lp = (w_idx + linearRand(0.0f, 1.0f)) * __dx + (h_idx + linearRand(0.0f, 1.0f)) * __dy + __d;
			vec3 hit2light = lp - hit_position;
			float len = length(hit2light);
			vec3 hit2light_dir = hit2light / len;

			if (scene_ptr->intersect(Ray(hit_position + EPSILON * hit2light_dir, hit2light_dir), len, __hash_code))
				continue;

			shade_idx ++;
			total_dir += hit2light_dir;
		}

		float tmp = shade_idx;
		shade_idx /= SAMPLES;
		return total_dir / tmp;
	}

	inline virtual void addPrimitives(vector<Primitive *> &primitives) {
		primitives.push_back(&__tri_a);
		primitives.push_back(&__tri_b);
	}
};

#endif