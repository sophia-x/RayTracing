#include <chrono>
#include <thread>
#include <glm/gtc/random.hpp>

#include "DepthCamera.hpp"

void DepthCamera::render(Mat &result, float f, float aperture_size, size_t sample_size, size_t thread_num) const {
	Size s = result.size();
	size_t width = s.width;
	size_t height = s.height;
	float pixel_size = __width_camera / width;

	float dist;
	unsigned long hash_code;

	auto begin = chrono::system_clock::now();
	vector<thread> t(thread_num);
	vector<vec3> colors(thread_num);

	for (unsigned short w = 0; w < width; w ++) {
		for (unsigned short h = 0; h < height; h ++) {
			float x_pos = w * pixel_size - __width_camera_half;
			float y_pos = __height_camera_half - h * pixel_size;
			vec3 origin(x_pos, y_pos, -IM_DIST);
			vec3 hit_point = origin / IM_DIST * f;

			vec3 color(0);
			for (unsigned short s = 0; s < sample_size; s ++) {
				for (unsigned i = 0; i < thread_num; i ++) {
					vec3 position = origin + vec3(pixel_size * circularRand(aperture_size), 0);
					vec3 direction = hit_point - position;

					vec3 w_position(__camera2world_matrix * vec4(position, 1.0f));
					vec3 w_direction = normalize(vec3(__camera2world_matrix * vec4(direction, 0)));
					t[i] = thread(thread_raytracing, Ray(w_position, w_direction), &colors[i], &hash_code, __scene_ptr);
				}

				for (unsigned i = 0; i < thread_num; i ++)
					t[i].join();

				for (unsigned i = 0; i < thread_num; i ++) {
					color += colors[i];
				}
			}

			color = glm::min(color / float(thread_num * sample_size), 1.0f);
			Vec3f &elem = result.at<Vec3f>(h, w);
			elem[0] = color[2];
			elem[1] = color[1];
			elem[2] = color[0];
		}
	}

	auto end = chrono::system_clock::now();
	std::chrono::duration<double> dur = end - begin;
	cout << dur.count() << " s" << endl;

	result.convertTo(result, CV_8UC3, 255.0);
}