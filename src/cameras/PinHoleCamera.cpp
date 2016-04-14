#include <glm/gtc/matrix_transform.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <limits>

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
			vec3 color = raytracing(camera2world_matrix * vec4(vec3(0.0f), 1.0f), normalize(camera2world_matrix * ray_direction), 0);

			Vec3f &elem = image.at<Vec3f>(h, w);
			elem[0] = color[2];
			elem[1] = color[1];
			elem[2] = color[0];
		}
	}

	resize(image, result, result.size(), 0, 0, INTER_CUBIC);
	imshow("RayTracing", result);
	waitKey();
}