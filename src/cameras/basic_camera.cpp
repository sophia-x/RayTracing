#include "../common.hpp"
#include "basic_camera.hpp"

vec3 BasicCamera::raytracing(const vec3 &ray_position, const vec3 &ray_direction, unsigned short recursive_count, float &min_t, unsigned long &hash_code) const {
	min_t = numeric_limits<float>::max();
	hash_code += recursive_count;

	if (recursive_count > scene_ptr->getMaxRecursive())
		return vec3(0.0);

	const vector<BasicModel*> &models = scene_ptr->getModels();
	const vector<BasicModel*> &lights = scene_ptr->getLights();
	const vec3 backgroud_color = scene_ptr->getBackgroundColor();

	float t;
	BasicModel *model_ptr = 0;
	vec3 normal; vec3 surface_color;
	vec3 tmp_nomal; vec3 tmp_color;

	for (vector<BasicModel *>::const_iterator it = models.begin(); it != models.end(); ++it) {
		if ((*it)->intersect(ray_position, ray_direction, t, tmp_nomal, tmp_color)) {
			if (t < min_t) {
				min_t = t;
				model_ptr = *it;
				normal = tmp_nomal;
				surface_color = tmp_color;
			}
		}
	}

	if (model_ptr == 0)
		return backgroud_color;
	hash_code += model_ptr->getHashCode();

	if (model_ptr->isLight())
		return model_ptr->getEmissionColor();

	vec3 color(0);
	float diffuse = model_ptr->getDiffuse();
	float specular = model_ptr->getSpecular();

	vec3 hit_position = ray_position + min_t * ray_direction;
	vec3 reflect_ray_dir = reflect(ray_direction, normal);

	for (vector<BasicModel *>::const_iterator it = lights.begin(); it != lights.end(); ++it) {
		vec3 light_center = (*it)->getCenter();
		vec3 hit2light = light_center - hit_position;
		float len = length(hit2light);
		vec3 hit2light_dir = hit2light / len;

		float shade_color = 1.0;
		for (vector<BasicModel *>::const_iterator model_it = models.begin(); model_it != models.end(); ++model_it) {
			if ((*it) == (*model_it))
				continue;

			if ((*model_it)->intersect(hit_position + EPSILON * hit2light_dir, hit2light_dir, t, tmp_nomal, tmp_color) && t < len) {
				shade_color = 0;
				break;
			}
		}

		if (shade_color == 0)
			continue;

		hash_code += (*it)->getHashCode();
		if (diffuse > 0) {
			float difuse_radio = dot(hit2light_dir, normal);
			if (difuse_radio > 0) {
				color += shade_color * difuse_radio * surface_color * (*it)->getEmissionColor() * diffuse;
			}
		}

		if (specular > 0) {
			float specular_radio = dot(reflect_ray_dir, hit2light_dir);
			if (specular_radio > 0) {
				color += glm::pow(specular_radio, model_ptr->getSpecularPower()) * shade_color * (*it)->getEmissionColor() * specular;
			}
		}
	}

	float reflection = model_ptr->getReflection();
	float dist;
	if (reflection > 0) {
		color += reflection * raytracing(hit_position + REFLACT_EPSILON * reflect_ray_dir, reflect_ray_dir, recursive_count + 1, dist, hash_code) * surface_color;
	}

	bool outside = dot(normal, -ray_direction) > 0 ? true : false;
	if (!outside) {
		normal = -normal;
	}
	float transparency = model_ptr->getTransparency();
	if (transparency > 0) {
		float refraction_radio = model_ptr->getRefractionRadio();
		float r_index = outside ? 1.0f / refraction_radio : refraction_radio;
		float cosI = -dot( normal, ray_direction );
		float cosT2 = 1.0f - r_index * r_index * (1.0f - cosI * cosI);
		if (cosT2 <= 0)
			return color;

		vec3 refract_dir = (r_index * ray_direction) + (r_index * cosI - sqrt( cosT2 )) * normal;

		vec3 refract_color = raytracing(hit_position + EPSILON * refract_dir, refract_dir, recursive_count + 1, dist, hash_code);
		vec3 absorbance_color = glm::exp( surface_color * model_ptr->getAbsorbance() * -dist);
		color += refract_color * absorbance_color;
	}

	return color;
}