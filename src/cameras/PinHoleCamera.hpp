#ifndef PIN_HOLE_CAMERA
#define PIN_HOLE_CAMERA

#include "basic_camera.hpp"

class PinHoleCamera: public BasicCamera {
public:
	PinHoleCamera(const vec4 &position, const vec4 &direction, const vec4 &up,
	              float fov, float radio, float im_dist): BasicCamera(position, direction, up, fov, radio, im_dist) {};

	void render(unsigned short width, unsigned short anti_t, Mat &result) const;
	vec3 raytracing(const vec4 &position, const vec4 &direction) const;
};

#endif