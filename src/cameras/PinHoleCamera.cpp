#include <glm/gtc/matrix_transform.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <limits>

#include <iostream>
#include <glm/gtx/string_cast.hpp>

#include "PinHoleCamera.hpp"

void PinHoleCamera::render(unsigned short width, unsigned short anti_t, Mat &result) const {
	mat4 camera2world_matrix = inverse(lookAt(vec3(position), vec3(position + direction), vec3(up)));

	float height_camera = 2 * tan(fov / 2) * im_dist;
	float width_camera = radio * height_camera;

	float pixel_size = width_camera / width / anti_t;

	unsigned short width_pixel = width * anti_t;
	unsigned short height_pixel = (unsigned short)(width_pixel / radio);

	Mat image(height_pixel, width_pixel, CV_32FC3);
	float width_camera_half = width_camera / 2;
	float height_camera_half = height_camera / 2;

	for (unsigned short w = 0; w < width_pixel; w ++) {
		for (unsigned short h = 0; h < height_pixel; h ++) {
			vec4 ray_direction = vec4(w * pixel_size - width_camera_half, height_camera_half - h * pixel_size, -im_dist, 0);
			vec3 color = raytracing(camera2world_matrix * vec4(vec3(0.0f), 1.0f), normalize(camera2world_matrix * ray_direction));

			Vec3f &elem = image.at<Vec3f>(h, w);
			elem[0] = std::min(color[2], 1.0f);
			elem[1] = std::min(color[1], 1.0f);
			elem[2] = std::min(color[0], 1.0f);
		}
	}

	resize(image, result, result.size(), 0, 0, INTER_CUBIC);
	imshow("RayTracing", result);
	waitKey();
}

vec3 PinHoleCamera::raytracing(const vec4 &ray_position, const vec4 &ray_direction) const {
	const vector<BasicModel*> &models = scene_ptr->models;
	const vector<BasicModel*> &lights = scene_ptr->lights;
	const vec3 backgroud_color = scene_ptr->backgroud_color;

	float t, min_t = numeric_limits<float>::max();
	BasicModel *model_ptr = 0;

	for (vector<BasicModel *>::const_iterator it = models.begin(); it != models.end(); ++it) {
		if ((*it)->intersect(ray_position, ray_direction, t)) {
			if (t < min_t) {
				min_t = t;
				model_ptr = *it;
			}
		}
	}

	if (model_ptr == 0)
		return backgroud_color;

	vec4 intersect_position = ray_position + vec4(min_t) * ray_direction;
	vec4 normal = model_ptr->normal(intersect_position);
	vec3 color = backgroud_color * scene_ptr->ambient_radio * model_ptr->surface_color(intersect_position);

	for (vector<BasicModel *>::const_iterator it = lights.begin(); it != lights.end(); ++it) {
		vec4 light_center = (*it)->get_center();
		vec4 position2light_dir = normalize(light_center - intersect_position);

		float difuse_radio = std::max(0.0f, dot(position2light_dir, normal));

		color += vec3(difuse_radio) * model_ptr->surface_color(intersect_position) * (*it)->emission_color();
	}

	return color;
}