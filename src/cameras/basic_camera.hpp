#ifndef BASIC_CAMERA
#define BASIC_CAMERA

#include "../common.hpp"
#include "../models/basic_model.hpp"
#include "../scenes/scene.hpp"

class BasicCamera {
protected:
	vec3 position, direction, up;
	float fov, radio, im_dist;

	const Scene *scene_ptr;

public:
	BasicCamera(const vec3 &position, const vec3 &direction, const vec3 &up, float fov, float radio, float im_dist):
		fov(fov), radio(radio), im_dist(im_dist), position(position), direction(direction), up(up) {}

	virtual ~BasicCamera() {}

	virtual void render(unsigned short width, unsigned short anti_t, Mat &result) const = 0;

	void setScene(const Scene *scene_p) {
		scene_ptr = scene_p;
	}

protected:
	vec3 raytracing(const vec3 &position, const vec3 &direction, unsigned short recusive_count, float &min_t, unsigned long &hash_code) const;
};

#endif