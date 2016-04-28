#include <chrono>
#include <thread>

#include "PinHoleCamera.hpp"

const size_t PinHoleCamera::SIZE = 8;
const short PinHoleCamera::DIR[][2] = {{ -1, -1}, {0, -1}, {1, -1}, { -1, 0}, { -1, 1}, {1, 0,}, {0, 1}, {1, 1}};

void PinHoleCamera::render(Mat &result, unsigned short anti_t, size_t thread_num) const {
	Size s = result.size();
	size_t width = s.width;
	size_t height = s.height;

	float pixel_size = __width_camera / width;
	short half_anti = anti_t / 2;
	float dist;

	Mat_<unsigned long> hash_code_mat(height, width, (unsigned long)0);

	auto begin = chrono::system_clock::now();

	vector<thread> t(thread_num);
	vector<vec3> colors(thread_num);

	for (unsigned short w = 0; w < width; w ++) {
		for (unsigned short h = 0; h < height; h += thread_num) {
			unsigned short num = std::min(thread_num, height - h);
			for (unsigned short dh = 0; dh < num; dh++) {
				float x_pos = w * pixel_size - __width_camera_half;
				float y_pos = __height_camera_half - (h + dh) * pixel_size;

				vec3 ray_direction(x_pos, y_pos, -IM_DIST);
				vec3 dir = normalize(vec3(__camera2world_matrix * vec4(ray_direction, 0)));
				t[dh] = thread(thread_raytracing, Ray(__world_position, dir), &colors[dh], &hash_code_mat(h + dh, w), __scene_ptr);
			}

			for (unsigned short dh = 0; dh < num; dh++)
				t[dh].join();

			for (unsigned short dh = 0; dh < num; dh++) {
				vec3 color = glm::min(colors[dh], 1.0f);
				Vec3f &elem = result.at<Vec3f>(h + dh, w);
				elem[0] = color[2];
				elem[1] = color[1];
				elem[2] = color[0];
			}
		}
	}

	thread_num = (2 * half_anti + 1) * (2 * half_anti + 1);
	vector<thread> t_anti(thread_num);
	vector<vec3> colors_anti(thread_num);
	unsigned long hash_code;

	for (unsigned short w = 0; w < width; w ++) {
		for (unsigned short h = 0; h < height; h ++) {
			bool same = true;
			for (size_t i = 0; i < SIZE; i++) {
				short p_h = (short)h + DIR[i][0];
				short p_w = (short)w + DIR[i][1];

				if (p_h >= 0 && p_h < height && p_w >= 0 && p_w < width && hash_code_mat(h, w) != hash_code_mat(p_h, p_w)) {
					same = false;
					break;
				}
			}

			if (!same) {
				float count = 0;
				float x_pos = w * pixel_size - __width_camera_half;
				float y_pos = __height_camera_half - h * pixel_size;

				for (short x_idx = -half_anti; x_idx <= half_anti; x_idx ++) {
					for (short y_idx = -half_anti; y_idx <= half_anti; y_idx ++) {
						vec3 dir = normalize(vec3(__camera2world_matrix * vec4( x_pos + x_idx * pixel_size / anti_t, y_pos + y_idx * pixel_size / anti_t, -IM_DIST, 0)));
						t_anti[count] = thread(thread_raytracing, Ray(__world_position, dir), &colors_anti[count], &hash_code, __scene_ptr);
						count ++;
					}
				}

				for (size_t i = 0; i < thread_num; i ++)
					t_anti[i].join();

				vec3 color(0);
				for (size_t i = 0; i < thread_num; i ++)
					color += colors_anti[i];
				color /= thread_num;

				color = glm::min(color, 1.0f);
				Vec3f &elem = result.at<Vec3f>(h, w);
				elem[0] = color[2];
				elem[1] = color[1];
				elem[2] = color[0];
			}
		}
	}

	auto end = chrono::system_clock::now();
	std::chrono::duration<double> dur = end - begin;
	cout << dur.count() << " s" << endl;

	result.convertTo(result, CV_8UC3, 255.0);
}