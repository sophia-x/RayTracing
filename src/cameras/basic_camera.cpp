#include "basic_camera.hpp"

#include <iostream>
#include <glm/gtx/string_cast.hpp>

vec3 BasicCamera::raytracing(const vec4 &ray_position, const vec4 &ray_direction, unsigned short recursive_count) const {
	if (recursive_count > scene_ptr->getMaxRecursive())
		return vec3(0.0);

	const vector<BasicModel*> &models = scene_ptr->getModels();
	const vector<BasicModel*> &lights = scene_ptr->getLights();
	const vec3 backgroud_color = scene_ptr->getBackgroundColor();

	float t, min_t = numeric_limits<float>::max();
	BasicModel *model_ptr = 0;
	vec4 normal; vec3 surface_color;
	vec4 tmp_nomal; vec3 tmp_color;

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

	if (model_ptr->isLight())
		return model_ptr->getEmissionColor();

	bool outside = dot(normal, -ray_direction) > 0 ? true : false;
	if (!outside) {
		normal = -normal;
	}

	vec3 color = backgroud_color * surface_color * scene_ptr->getAmbientRadio();
	float diffuse = model_ptr->getDiffuse();
	float specular = model_ptr->getSpecular();

	vec4 hit_position = ray_position + min_t * ray_direction;
	vec4 reflect_ray_dir = reflect(ray_direction, normal);

	for (vector<BasicModel *>::const_iterator it = lights.begin(); it != lights.end(); ++it) {
		vec4 light_center = (*it)->getCenter();
		vec4 hit2light_dir = normalize(light_center - hit_position);

		vec3 shade_color(1.0f);
		for (vector<BasicModel *>::const_iterator model_it = models.begin(); model_it != models.end(); ++model_it) {
			if ((*it) == (*model_it))
				continue;

			if ((*model_it)->intersect(hit_position + EPSILON * hit2light_dir, hit2light_dir, t, tmp_nomal, tmp_color)) {
				shade_color *= scene_ptr->getTransparencyPower() * (*model_it)->getTransparency() * tmp_color;
			}
		}

		if (length(shade_color) < numeric_limits<float>::epsilon())
			continue;

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

	float facingratio = -dot(ray_direction, normal);
	float fresneleffect = mix (glm::pow(1 - facingratio, scene_ptr->getFresPower()), 1.0f, model_ptr->getReflectRadio());
	float reflection = model_ptr->getReflection();
	if (reflection > 0) {
		color += fresneleffect * reflection * raytracing(hit_position + EPSILON * reflect_ray_dir, reflect_ray_dir, recursive_count + 1) * surface_color;
	}

	float transparency = model_ptr->getTransparency();
	if (transparency > 0) {
		float refraction_radio = model_ptr->getRefractionRadio();
		vec4 refract_dir = refract(ray_direction, normal, outside ? 1.0f / refraction_radio : refraction_radio);
		color += (1 - fresneleffect) * raytracing(hit_position + EPSILON * refract_dir, refract_dir, recursive_count + 1) * transparency * surface_color;
	}

	return color;
}