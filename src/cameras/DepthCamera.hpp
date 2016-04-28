#ifndef DEPTH_CAMERA
#define DEPTH_CAMERA

#include "basic_camera.hpp"
#include "../common.hpp"

class DepthCamera: public BasicCamera {
public:
	DepthCamera(const vec3 &position, const vec3 &direction, const vec3 &up,
	            float fov, float radio): BasicCamera(position, direction, up, fov, radio) {};

	void render(Mat &result, float f, float aperture_size, size_t sample_size, size_t thread_num) const;
};

#endif