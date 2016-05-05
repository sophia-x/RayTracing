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
	PointLight(const vec3 &center, float radius, const vec3 &emission_color): Light(Material(emission_color, true), emission_color),
		__sphere(center, radius, __hash_code, __material) {}

	inline float calcShade(const Scene *scene_ptr, const vec3 &hit_position, const vec3 &normal, const vec3 &reflect_ray_dir, const Material &material, const vec3 &surface_color, vec3 &color) const {
		vec3 hit2light = __sphere.getCenter() - hit_position;
		float len = length(hit2light);
		vec3 hit2light_dir = hit2light / len;

		if (scene_ptr->intersect(Ray(hit_position + EPSILON * hit2light_dir, hit2light_dir), len, __hash_code))
			return 0;

		float diffuse = material.getDiffuse();
		if (diffuse > 0) {
			float difuse_radio = dot(hit2light_dir, normal);
			if (difuse_radio > 0) {
				color += difuse_radio * surface_color * __emission_color * diffuse;
			}
		}

		float specular = material.getSpecular();
		if (specular > 0) {
			float specular_radio = dot(reflect_ray_dir, hit2light_dir);
			if (specular_radio > 0) {
				color += glm::pow(specular_radio, material.getSpecularPower()) * __emission_color * specular;
			}
		}

		return 1;
	}

	inline virtual void addPrimitives(vector<Primitive *> &primitives) {
		primitives.push_back(&__sphere);
	}
};

#endif