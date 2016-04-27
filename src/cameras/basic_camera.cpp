#include "basic_camera.hpp"

const float BasicCamera::IM_DIST = 1.0f;

vec3 BasicCamera::raytracing(const Ray &ray, unsigned short recursive_count, float &min_t, unsigned long &hash_code) const {
	min_t = numeric_limits<float>::max();

	if (recursive_count > __scene_ptr->getMaxRecursive()) {
		hash_code += recursive_count;
		return vec3(0.0);
	}

	const vector<const Light *> &lights = __scene_ptr->getLights();
	const vec3 &backgroud_color = __scene_ptr->getBackgroundColor();

	Primitive const* hit_ptr = 0;
	vec3 normal; vec3 surface_color;

	if (!__scene_ptr->intersect(ray, min_t, normal, surface_color, hit_ptr))
		return backgroud_color;

	hash_code += recursive_count * hit_ptr->getHashCode();
	const Material &material = hit_ptr->getMaterial();

	if (material.isLight())
		return hit_ptr->getSurfaceColor();

	vec3 color(0);
	float diffuse = material.getDiffuse();
	float specular = material.getSpecular();

	const vec3 &ray_position = ray.getPosition();
	const vec3 &ray_direction = ray.getDirection();
	vec3 hit_position = ray_position + min_t * ray_direction;
	vec3 reflect_ray_dir = reflect(ray_direction, normal);

	for (auto it = lights.begin(); it != lights.end(); ++it) {
		const vec3 &light_center = (*it)->getCenter();
		vec3 hit2light = light_center - hit_position;
		float len = length(hit2light);
		vec3 hit2light_dir = hit2light / len;

		if (__scene_ptr->intersect(Ray(hit_position + EPSILON * hit2light_dir, hit2light_dir), len, (*it)->getHashCode()))
			continue;

		hash_code += (*it)->getHashCode() * recursive_count;
		if (diffuse > 0) {
			float difuse_radio = dot(hit2light_dir, normal);
			if (difuse_radio > 0) {
				color += difuse_radio * surface_color * (*it)->getEmissionColor() * diffuse;
			}
		}

		if (specular > 0) {
			float specular_radio = dot(reflect_ray_dir, hit2light_dir);
			if (specular_radio > 0) {
				color += glm::pow(specular_radio, material.getSpecularPower()) * (*it)->getEmissionColor() * specular;
			}
		}
	}

	float reflection = material.getReflection();
	float dist;
	if (reflection > 0) {
		color += reflection * raytracing(Ray(hit_position + REFLACT_EPSILON * reflect_ray_dir, reflect_ray_dir), recursive_count + 1, dist, hash_code) * surface_color;
	}

	bool outside = dot(normal, -ray_direction) > 0 ? true : false;
	if (!outside) {
		normal = -normal;
	}
	float transparency = material.getTransparency();
	if (transparency > 0) {
		float refraction_radio = material.getRefractionRadio();
		float r_index = outside ? 1.0f / refraction_radio : refraction_radio;
		float cosI = -dot( normal, ray_direction );
		float cosT2 = 1.0f - r_index * r_index * (1.0f - cosI * cosI);
		if (cosT2 <= 0)
			return color;

		vec3 refract_dir = (r_index * ray_direction) + (r_index * cosI - sqrt( cosT2 )) * normal;

		vec3 refract_color = raytracing(Ray(hit_position + EPSILON * refract_dir, refract_dir), recursive_count + 1, dist, hash_code);
		vec3 absorbance_color = glm::exp( surface_color * material.getAbsorbance() * -dist);
		color += refract_color * absorbance_color;
	}

	return color;
}