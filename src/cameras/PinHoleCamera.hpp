#ifndef PIN_HOLE_CAMERA
#define PIN_HOLE_CAMERA

#include "basic_camera.hpp"

class PinHoleCamera:public BasicCamera{
public:
	PinHoleCamera(const vec3 &position, const vec3 &direction, const vec3 &up,
				float fov, float radio, float im_dist):BasicCamera(position, direction, up, fov, radio, im_dist){};

	void render(const vector<BasicModel*> &models, unsigned short width, unsigned short anti_t) const;
	vec3 raytracing(const vec3 &position, const vec3 &direction, const vector<BasicModel*> &models, const vector<unsigned int> &lights_idx) const;
};

#endif