#ifndef PIN_HOLE_CAMERA
#define PIN_HOLE_CAMERA

#include "basic_camera.hpp"

class PinHoleCamera: public BasicCamera {
private:
	static const size_t SIZE;
	static const short DIR[][2];

public:
	PinHoleCamera(const vec4 &position, const vec4 &direction, const vec4 &up,
	              float fov, float radio, float im_dist): BasicCamera(position, direction, up, fov, radio, im_dist) {};

	void render(unsigned short width, unsigned short anti_t, Mat &result) const;
};

#endif