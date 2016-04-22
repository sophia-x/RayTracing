#ifndef PIN_HOLE_CAMERA
#define PIN_HOLE_CAMERA

#include "basic_camera.hpp"
#include "../common.hpp"

class PinHoleCamera: public BasicCamera {
private:
	static const size_t SIZE;
	static const short DIR[][2];

public:
	PinHoleCamera(const vec3 &position, const vec3 &direction, const vec3 &up,
	              float fov, float radio): BasicCamera(position, direction, up, fov, radio) {};

	void render(Mat &result, unsigned short anti_t) const;
};

#endif