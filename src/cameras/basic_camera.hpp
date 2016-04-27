#ifndef BASIC_CAMERA
#define BASIC_CAMERA

#include "Ray.hpp"
#include "../common.hpp"
#include "../scenes/scene.hpp"

vec3 raytracing(const Ray &ray, unsigned short recusive_count, float &min_t, unsigned long &hash_code, Scene const* scene_ptr);

class BasicCamera {
protected:
	static const float IM_DIST;

	vec3 __position, __direction, __up;
	float __fov, __radio;

	const Scene *__scene_ptr;

public:
	BasicCamera(const vec3 &position, const vec3 &direction, const vec3 &up, float fov, float radio):
		__fov(fov), __radio(radio), __position(position), __direction(direction), __up(up), __scene_ptr(0) {}

	virtual ~BasicCamera() {}

	virtual void render(Mat &result, unsigned short anti_t, size_t thread_num) const = 0;

	inline void setScene(const Scene *scene_ptr) {
		__scene_ptr = scene_ptr;
	}

	inline size_t getHeight(size_t width) const {
		return size_t(width / __radio);
	}
};

#endif