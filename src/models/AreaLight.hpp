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

	inline float calcShade(const Scene *scene_ptr, const vec3 &hit_position, const vec3 &normal, const vec3 &reflect_ray_dir, const Material &material, const vec3 &surface_color, vec3 &color) const {
		float diffuse = material.getDiffuse();
		float specular = material.getSpecular();
		if (diffuse <= EPSILON && specular <= EPSILON)
			return 0;

		float shade_idx = 0;
		// Monte Carlo rendering
		float dx = __width / SHADE_SIZE, dy = __height / SHADE_SIZE;

		vec3 t_color(0);
		for (size_t i = 0; i < SAMPLES; i ++) {
			int idx = i % SHADE_SIZE_2;
			int w_idx = idx / SHADE_SIZE, h_idx = idx % SHADE_SIZE;
			float w = __min_p[0] + w_idx * dx, h = __min_p[2] + h_idx * dy;

			vec3 lp(w + linearRand(0.0f, dx), __min_p[1], h + linearRand(0.0f, dy) );
			vec3 hit2light = lp - hit_position;
			float len = length(hit2light);
			vec3 hit2light_dir = hit2light / len;

			if (scene_ptr->intersect(Ray(hit_position + EPSILON * hit2light_dir, hit2light_dir), len, __hash_code))
				continue;

			shade_idx ++;
			if (diffuse > 0) {
				float difuse_radio = dot(hit2light_dir, normal);
				if (difuse_radio > 0) {
					t_color += difuse_radio * surface_color * __emission_color * diffuse;
				}
			}

			if (specular > 0) {
				float specular_radio = dot(reflect_ray_dir, hit2light_dir);
				if (specular_radio > 0) {
					t_color += glm::pow(specular_radio, material.getSpecularPower()) * __emission_color * specular;
				}
			}
		}

		color += t_color / float(SAMPLES);
		return shade_idx / SAMPLES;
	}

	inline virtual void addPrimitives(vector<const Primitive *> &primitives) const {
		primitives.push_back(&__tri_a);
		primitives.push_back(&__tri_b);
	}
};

#endif