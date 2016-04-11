#include <glm/gtc/matrix_transform.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "PinHoleCamera.hpp"

using namespace cv;

void PinHoleCamera::render(const vector<BasicModel*> &models, unsigned short width, unsigned short anti_t) const{
	mat4 camera2world_matrix = inverse(lookAt(position, position+direction, up));

	float height_camera = 2*tan(fov/2)*im_dist;
	float width_camera = radio*height_camera;

	float pixel_size = width_camera/width/anti_t;

	unsigned short width_pixel = width*anti_t;
	unsigned short height_pixel = (unsigned short)(width_pixel/radio);

	Mat image(height_pixel, width_pixel, CV_32FC3);
	float width_camera_half = width_camera/2;
	float height_camera_half = height_camera/2;

	for(unsigned short w = 0;w < width_pixel;w ++){
		for(unsigned short h = 0;h < height_pixel;h ++){
			vec3 ray_direction = vec3(w*pixel_size - width_camera_half, height_camera_half - h*pixel_size, -im_dist);
			vec3 color = raytracing(vec3(camera2world_matrix*vec4(vec3(0.0f),1.0f)), vec3(camera2world_matrix*vec4(ray_direction,0.0f)), models, std::vector<unsigned int>());

			Vec3f &elem = image.at<Vec3f>(h, w);
			elem[0] = color[2];
			elem[1] = color[1];
			elem[2] = color[0];
		}
	}

	Mat result(height_pixel/anti_t, width, CV_32FC3);
	resize(image, result, result.size(), 0, 0, INTER_CUBIC);
	imshow("loveLena", result);
    waitKey();
}

vec3 PinHoleCamera::raytracing(const vec3 &ray_position, const vec3 &ray_direction, const vector<BasicModel*> &models, const vector<unsigned int> &lights_idx) const{
	float t;
	if (models[0]->intersect(ray_position, ray_direction, t))
		return vec3(1.0f, 0.0f, 0.0f);
	return vec3(0.0f);
}