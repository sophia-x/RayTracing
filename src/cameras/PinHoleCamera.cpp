#include <chrono>
#include <thread>

#include "PinHoleCamera.hpp"

const static size_t DIR_SIZE = 8;
const static short DIR[][2] = {{ -1, -1}, {0, -1}, {1, -1}, { -1, 0}, { -1, 1}, {1, 0,}, {0, 1}, {1, 1}};

void thread_pool_1stpass(mutex *mutex_w_h, unsigned short *w_t, unsigned short *h_t, unsigned short width,
                         unsigned short height, float width_camera_half, float height_camera_half, float pixel_size,
                         const vec3 &world_position, const mat4 &camera2world_matrix, Mat *result, Mat_<unsigned long> *hash_code_mat, Scene const* scene_ptr) {
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

		vec3 ray_direction(x_pos, y_pos, -IM_DIST);
		vec3 dir = normalize(vec3(camera2world_matrix * vec4(ray_direction, 0)));
		float min_t;
		vec3 color = raytracing(Ray(world_position, dir), 0, min_t, (*hash_code_mat)(h, w), scene_ptr);
		color = min(color, 1.0f);
		Vec3f &elem = (*result).at<Vec3f>(h, w);
		elem[0] = color[2];
		elem[1] = color[1];
		elem[2] = color[0];
	}
}

void thread_pool_2ndpass(mutex *mutex_w_h, unsigned short *w_t, unsigned short *h_t, unsigned short width,
                         unsigned short height, float width_camera_half, float height_camera_half, float pixel_size, short half_anti,
                         const vec3 &world_position, const mat4 &camera2world_matrix, Mat *result, const Mat_<unsigned long> &hash_code_mat, Scene const* scene_ptr) {
	short w, h;
	short anti_t = half_anti * 2 + 1;
	short anti_t_2 = anti_t * anti_t;
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

		bool same = true;
		for (size_t i = 0; i < DIR_SIZE; i++) {
			short p_h = h + DIR[i][0];
			short p_w = w + DIR[i][1];

			if (p_h >= 0 && p_h < height && p_w >= 0 && p_w < width && hash_code_mat(h, w) != hash_code_mat(p_h, p_w)) {
				same = false;
				break;
			}
		}

		if (!same) {
			float x_pos = w * pixel_size - width_camera_half;
			float y_pos = height_camera_half - h * pixel_size;

			vec3 color(0);
			unsigned long hash_code;
			float min_t;
			for (short x_idx = -half_anti; x_idx <= half_anti; x_idx ++) {
				for (short y_idx = -half_anti; y_idx <= half_anti; y_idx ++) {
					vec3 dir = normalize(vec3(camera2world_matrix * vec4( x_pos + x_idx * pixel_size / anti_t, y_pos + y_idx * pixel_size / anti_t, -IM_DIST, 0)));
					color += raytracing(Ray(world_position, dir), 0, min_t, hash_code, scene_ptr);
				}
			}

			color /= anti_t_2;
			color = min(color, 1.0f);
			Vec3f &elem = (*result).at<Vec3f>(h, w);
			elem[0] = color[2];
			elem[1] = color[1];
			elem[2] = color[0];
		}
	}
}

void PinHoleCamera::render(Mat &result, unsigned short anti_t, size_t thread_num) const {
	Size s = result.size();
	size_t width = s.width;
	size_t height = s.height;

	float pixel_size = __width_camera / width;
	short half_anti = anti_t / 2;
	float dist;

	Mat_<unsigned long> hash_code_mat(height, width, (unsigned long)0);

	vector<thread> ts(thread_num);
	mutex mutex_w_h;
	unsigned short w_t = 0, h_t = 0;
	for (int i = 0; i < thread_num; i ++) {
		ts[i] = thread(thread_pool_1stpass, &mutex_w_h, &w_t, &h_t, width, height, __width_camera_half, __height_camera_half, pixel_size,
		               __world_position, __camera2world_matrix, &result, &hash_code_mat, __scene_ptr);
	}

	auto begin = chrono::system_clock::now();

	for (int i = 0; i < thread_num; i ++)
		ts[i].join();

	auto end = chrono::system_clock::now();
	std::chrono::duration<double> dur_1st = end - begin;
	cout << "1st pass:" << dur_1st.count() << " s" << endl;

	if (anti_t > 1) {
		unsigned short w_t = 0, h_t = 0;
		for (int i = 0; i < thread_num; i ++) {
			ts[i] = thread(thread_pool_2ndpass, &mutex_w_h, &w_t, &h_t, width, height, __width_camera_half, __height_camera_half, pixel_size,
			               half_anti, __world_position, __camera2world_matrix, &result, hash_code_mat, __scene_ptr);
		}

		begin = chrono::system_clock::now();

		for (int i = 0; i < thread_num; i ++)
			ts[i].join();

		end = chrono::system_clock::now();
		std::chrono::duration<double> dur_2nd = end - begin;
		cout << "2nd pass:" << dur_2nd.count() << " s" << endl;
		cout << "total:" << dur_1st.count() + dur_2nd.count() << " s" << endl;
	}

	result.convertTo(result, CV_8UC3, 255.0);
}