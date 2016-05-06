#ifndef TEXTURE
#define TEXTURE

#include "../common.hpp"

class Texture {
private:
	Mat __image;
	size_t __width, __height;

	Mat __normals;
	bool __has_normals;
	float __n_radio;

public:
	Texture(const char *file_name): __has_normals(false) {
		Mat image = imread(file_name, CV_LOAD_IMAGE_COLOR);
		Size s = image.size();
		__width = s.width;
		__height = s.height;
		__image = Mat(__height, __width, CV_32FC3);

		image.convertTo(__image, CV_32FC3, 1.0 / 255);
	}

	Texture(const char *file_name, const char *normal_name, float n_radio): __has_normals(true), __n_radio(n_radio) {
		Mat image = imread(file_name, CV_LOAD_IMAGE_COLOR);
		Size s = image.size();
		__width = s.width;
		__height = s.height;
		__image = Mat(__height, __width, CV_32FC3);
		image.convertTo(__image, CV_32FC3, 1.0 / 255);

		Mat normals = imread(normal_name, CV_LOAD_IMAGE_COLOR);
		Mat normals_f = Mat(normals.size(), CV_32FC3);
		normals.convertTo(normals_f, CV_32FC3, 1.0 / 255);
		__normals = Mat(__height, __width, CV_32FC3);
		resize(normals_f, __normals, s, INTER_CUBIC);
	}

	inline void getColorNormal(float u, float v, vec3 &color, vec3 &normal) const {
		// fetch a bilinearly filtered texel
		float fu = (u + 1000.0f) * __width;
		float fv = (v + 1000.0f) * __width;

		int u1 = ((int)fu) % __width;
		int v1 = ((int)fv) % __height;
		int u2 = (u1 + 1) % __width;
		int v2 = (v1 + 1) % __height;
		// calculate fractional parts of u and v
		float fracu = fu - floorf( fu );
		float fracv = fv - floorf( fv );
		// calculate weight factors
		float w1 = (1 - fracu) * (1 - fracv);
		float w2 = fracu * (1 - fracv);
		float w3 = (1 - fracu) * fracv;
		float w4 = fracu *  fracv;

		// scale and sum the four colors
		Vec3f tmp = __image.at<Vec3f>(v1, u1) * w1 + __image.at<Vec3f>(v1, u2) * w2 + __image.at<Vec3f>(v2, u1) * w3 + __image.at<Vec3f>(v2, u2) * w4;
		color = vec3(tmp[2], tmp[1], tmp[0]);

		if (__has_normals) {
			Vec3f tmp_n = __normals.at<Vec3f>(v1, u1) * w1 + __normals.at<Vec3f>(v1, u2) * w2 + __normals.at<Vec3f>(v2, u1) * w3 + __normals.at<Vec3f>(v2, u2) * w4;
			normal = vec3(tmp_n[0], tmp_n[1], tmp_n[2]) * __n_radio;
		} else {
			normal = vec3(0);
		}
	}
};

#endif