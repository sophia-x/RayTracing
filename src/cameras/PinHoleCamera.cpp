#include <glm/gtc/matrix_transform.hpp>
#include <limits>
#include <iostream>
#include <chrono>

#include "PinHoleCamera.hpp"

using namespace std;

const size_t PinHoleCamera::SIZE = 4;
const short PinHoleCamera::DIR[][2] = {{ -1, -1}, {0, -1}, {1, -1}, { -1, 0}};

void PinHoleCamera::render(unsigned short width, unsigned short anti_t, Mat &result) const {
	mat4 camera2world_matrix = inverse(lookAt(position, position + direction, up));

	float height_camera = 2 * tan(fov / 2) * im_dist;
	float width_camera = radio * height_camera;

	float pixel_size = width_camera / width;

	unsigned short height = (unsigned short)(width / radio);

	Mat_<unsigned long> hash_code_mat(height, width, (unsigned long)0);

	float width_camera_half = width_camera / 2;
	float height_camera_half = height_camera / 2;
	short half_anti = anti_t / 2;

	float dist;
	vec3 world_position = vec3(camera2world_matrix * vec4(vec3(0.0f), 1.0f));

	auto begin = chrono::system_clock::now();

	for (unsigned short w = 0; w < width; w ++) {
		for (unsigned short h = 0; h < height; h ++) {
			float x_pos = w * pixel_size - width_camera_half;
			float y_pos = height_camera_half - h * pixel_size;

			vec3 ray_direction(x_pos, y_pos, -im_dist);
			vec3 color = raytracing(world_position, normalize(vec3(camera2world_matrix * vec4(ray_direction, 0))), 0, dist, hash_code_mat(h, w));

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
				float count = 1;
				for (short x_idx = -half_anti; x_idx <= half_anti; x_idx ++) {
					for (short y_idx = -half_anti; y_idx <= half_anti; y_idx ++) {
						if (x_idx == 0 && y_idx == 0)
							continue;
						count ++;
						color += raytracing(world_position, normalize(vec3(camera2world_matrix * vec4( x_pos + x_idx * pixel_size / anti_t, y_pos + y_idx * pixel_size / anti_t, -im_dist, 0))), 0, dist, hash_code_mat(h, w));
					}
				}
				color /= count;
			}

			color = glm::min(color, 1.0f);
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