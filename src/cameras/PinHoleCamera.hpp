#ifndef PIN_HOLE_CAMERA
#define PIN_HOLE_CAMERA

#include "basic_camera.hpp"
#include "../common.hpp"

class PinHoleCamera: public BasicCamera {
public:
	PinHoleCamera(const vec3 &position, const vec3 &direction, const vec3 &up,
	              float fov, float radio): BasicCamera(position, direction, up, fov, radio) {};

	void render(Mat &result, unsigned short anti_t, size_t thread_num = 16) const;
};

#endif