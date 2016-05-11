#include <chrono>
#include <thread>
#include <glm/gtc/random.hpp>

#include "DepthCamera.hpp"

void thread_pool(mutex *mutex_w_h, unsigned short *w_t, unsigned short *h_t, unsigned short width,
                 unsigned short height, float width_camera_half, float height_camera_half, float pixel_size, float f, float aperture_size, size_t sample_size,
                 const mat4 &camera2world_matrix, Mat *result, Scene const* scene_ptr) {
	unsigned short w, h;
	while (true) {
		(*mutex_w_h).lock();
		{
			if ((*h_t) >= height) {
				(*h_t) = 0;
				(*w_t) ++;
			}
			if ((*w_t) >= width) {
				(*mutex_w_h).unlock();
				return;
			}
			w = (*w_t); h = (*h_t);
			(*h_t) ++;
		}
		(*mutex_w_h).unlock();

		float x_pos = w * pixel_size - width_camera_half;
		float y_pos = height_camera_half - h * pixel_size;
		vec3 origin(x_pos, y_pos, -IM_DIST);
		vec3 hit_point = origin / IM_DIST * f;

		vec3 color(0);
		float min_t;
		unsigned long hash_code;
		for (unsigned short s = 0; s < sample_size; s ++) {
			vec3 position = origin + vec3(pixel_size * circularRand(aperture_size), 0);
			vec3 direction = hit_point - position;

			vec3 w_position(camera2world_matrix * vec4(position, 1.0f));
			vec3 w_direction = normalize(vec3(camera2world_matrix * vec4(direction, 0)));

			color += raytracing(Ray(w_position, w_direction), 0, min_t, hash_code, scene_ptr);
		}

		color = glm::min(color / float(sample_size), 1.0f);
		Vec3f &elem = (*result).at<Vec3f>(h, w);
		elem[0] = color[2];
		elem[1] = color[1];
		elem[2] = color[0];
	}
}

void DepthCamera::render(Mat &result, float f, float aperture_size, size_t sample_size, size_t thread_num) const {
	Size s = result.size();
	size_t width = s.width;
	size_t height = s.height;
	float pixel_size = __width_camera / width;

	vector<thread> ts(thread_num);
	mutex mutex_w_h;
	unsigned short w_t = 0, h_t = 0;
	for (int i = 0; i < thread_num; i ++) {
		ts[i] = thread(thread_pool, &mutex_w_h, &w_t, &h_t, width, height, __width_camera_half, __height_camera_half, pixel_size,
		               f, aperture_size, sample_size, __camera2world_matrix, &result, __scene_ptr);
	}

	auto begin = chrono::system_clock::now();

	for (int i = 0; i < thread_num; i ++)
		ts[i].join();

	auto end = chrono::system_clock::now();
	std::chrono::duration<double> dur = end - begin;
	cout << "total:" << dur.count() << " s" << endl;

	result.convertTo(result, CV_8UC3, 255.0);
}