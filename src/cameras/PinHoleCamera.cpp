#include <glm/gtc/matrix_transform.hpp>
#include <limits>

#include "PinHoleCamera.hpp"

#define SIZE 4
short DIR[2][SIZE] = {{ -1, 0, 1, -1}, { -1, -1, -1, 0}};

void PinHoleCamera::render(unsigned short width, unsigned short anti_t, Mat &result) const {
	mat4 camera2world_matrix = inverse(lookAt(vec3(position), vec3(position + direction), vec3(up)));

	float height_camera = 2 * tan(fov / 2) * im_dist;
	float width_camera = radio * height_camera;

	float pixel_size = width_camera / width;

	unsigned short height = (unsigned short)(width / radio);

	Mat_<unsigned long> hash_code_mat(height, width, (unsigned long)0);

	float width_camera_half = width_camera / 2;
	float height_camera_half = height_camera / 2;
	short half_anti = anti_t / 2;

	float dist;
	vec4 world_position = camera2world_matrix * vec4(vec3(0.0f), 1.0f);
	for (unsigned short w = 0; w < width; w ++) {
		for (unsigned short h = 0; h < height; h ++) {
			float x_pos = w * pixel_size - width_camera_half;
			float y_pos = height_camera_half - h * pixel_size;

			vec4 ray_direction = vec4(x_pos, y_pos, -im_dist, 0);
			vec3 color = raytracing(world_position, normalize(camera2world_matrix * ray_direction), 0, dist, hash_code_mat(h, w));

			bool same = true;
			for (size_t i = 0; i < SIZE; i++) {
				short p_h = (short)h + DIR[0][i];
				short p_w = (short)w + DIR[1][i];

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
						color += raytracing(world_position, normalize(camera2world_matrix * vec4( x_pos + x_idx * pixel_size / anti_t, y_pos + y_idx * pixel_size / anti_t, -im_dist, 0)), 0, dist, hash_code_mat(h, w));
					}
				}
				color /= count;
			}

			Vec3f &elem = result.at<Vec3f>(h, w);
			elem[0] = color[2];
			elem[1] = color[1];
			elem[2] = color[0];
		}
	}
}