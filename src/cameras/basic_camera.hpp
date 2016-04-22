#ifndef BASIC_CAMERA
#define BASIC_CAMERA

#include "../common.hpp"
#include "../models/basic_model.hpp"
#include "../scenes/scene.hpp"

class BasicCamera {
protected:
	static const float IM_DIST;

	vec3 position, direction, up;
	float fov, radio;

	const Scene *scene_ptr;

public:
	BasicCamera(const vec3 &position, const vec3 &direction, const vec3 &up, float fov, float radio):
		fov(fov), radio(radio), position(position), direction(direction), up(up), scene_ptr(0) {}

	virtual ~BasicCamera() {}

	virtual void render(Mat &result, unsigned short anti_t) const = 0;

	inline void setScene(const Scene *scene_p) {
		scene_ptr = scene_p;
	}

	inline size_t getHeight(size_t width) const {
		return size_t(width / radio);
	}

protected:
	vec3 raytracing(const vec3 &position, const vec3 &direction, unsigned short recusive_count, float &min_t, unsigned long &hash_code) const;
};

#endif