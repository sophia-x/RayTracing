#ifndef TEXTURE
#define TEXTURE

#include "../common.hpp"

class Texture {
private:
	Mat __image;
	size_t __width, __height;

public:
	Texture(const char *file_name) {
		Mat image = imread(file_name, CV_LOAD_IMAGE_COLOR);
		Size s = image.size();
		__width = s.width;
		__height = s.height;
		__image = Mat(__height, __width, CV_32FC3);

		image.convertTo(__image, CV_32FC3, 1.0 / 255);
	}

	inline vec3 getUvColor(float u, float v) const {
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
		return vec3(tmp[2], tmp[1], tmp[0]);
	}
};

#endif