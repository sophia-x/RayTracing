#include "basic_camera.hpp"

vec3 raytracing(const Ray &ray, unsigned short recursive_count, float &min_t, unsigned long &hash_code, Scene const* scene_ptr) {
	min_t = numeric_limits<float>::max();

	hash_code += recursive_count;

	if (recursive_count > scene_ptr->getMaxRecursive())
		return vec3(0.0);

	const vector<const Light *> &lights = scene_ptr->getLights();
	const vec3 &backgroud_color = scene_ptr->getBackgroundColor();

	Primitive * hit_ptr = 0;

	if (!scene_ptr->intersect(ray, min_t, hit_ptr))
		return backgroud_color;

	hash_code += hit_ptr->getHashCode();
	const Material &material = hit_ptr->getMaterial();
	const vec3 &ray_position = ray.getPosition();
	const vec3 &ray_direction = ray.getDirection();
	vec3 hit_position = ray_position + min_t * ray_direction;
	vec3 normal, surface_color;
	hit_ptr->getColorNormal(hit_position, surface_color, normal);

	if (material.isLight())
		return surface_color;

	vec3 color(0);

	float diffuse = material.getDiffuse();
	float specular = material.getSpecular();

	if (diffuse >= EPSILON || specular >= EPSILON) {
		float shade_idx;
		for (auto it = lights.begin(); it != lights.end(); ++it) {
			vec3 mean_dir = (*it)->calcShade(scene_ptr, hit_position, shade_idx);
			if (shade_idx <= EPSILON)
				continue;

			hash_code += (unsigned long)((*it)->getHashCode() * shade_idx);
			const vec3 &emission_color = (*it)->getEmissionColor();

			if (diffuse > 0) {
				float difuse_radio = dot(mean_dir, normal);
				if (difuse_radio > 0) {
					color += difuse_radio * surface_color * emission_color * diffuse * shade_idx;
				}
			}

			if (specular > 0) {
				vec3 R = mean_dir - 2.0f * dot( mean_dir, normal ) * normal;
				float radio = dot( ray_direction, R );
				if (radio > 0) {
					float specular_idx = material.getSpecularIdx();
					float spec = radio * specular * shade_idx / (specular_idx - specular_idx * radio + radio);
					// add specular component to ray color
					color += spec * emission_color;
				}
			}
		}
	}

	float reflection = material.getReflection();
	float dist;
	if (reflection > 0) {
		vec3 reflect_ray_dir = reflect(ray_direction, normal);
		color += reflection * raytracing(Ray(hit_position + REFLACT_EPSILON * reflect_ray_dir, reflect_ray_dir), recursive_count + 1, dist, hash_code, scene_ptr) * surface_color;
	}

	bool outside = -dot(normal, ray_direction) > 0 ? true : false;
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

		vec3 refract_color = raytracing(Ray(hit_position + EPSILON * refract_dir, refract_dir), recursive_count + 1, dist, hash_code, scene_ptr);
		vec3 absorbance_color = glm::exp( surface_color * material.getAbsorbance() * -dist);
		color += refract_color * absorbance_color * transparency;
	}

	return color;
}