#ifndef BASIC_CAMERA
#define BASIC_CAMERA

#include <glm/gtc/matrix_transform.hpp>

#include "Ray.hpp"
#include "../common.hpp"
#include "../scenes/scene.hpp"

static const float IM_DIST = 1.0f;

vec3 raytracing(const Ray &ray, unsigned short recusive_count, float &min_t, unsigned long &hash_code, Scene const* scene_ptr);

class BasicCamera {
protected:
	vec3 __position, __direction, __up;
	float __fov, __radio;
	const Scene *__scene_ptr;

	float __width_camera, __width_camera_half, __height_camera_half;
	mat4 __camera2world_matrix;
	vec3 __world_position;

public:
	BasicCamera(const vec3 &position, const vec3 &direction, const vec3 &up, float fov, float radio):
		__fov(fov), __radio(radio), __position(position), __direction(direction), __up(up), __scene_ptr(0) {
		float height_camera = 2 * tan(__fov / 2) * IM_DIST;
		__width_camera = __radio * height_camera;
		__width_camera_half = __width_camera / 2;
		__height_camera_half = height_camera / 2;

		__camera2world_matrix = inverse(lookAt(__position, __position + __direction, __up));
		__world_position = vec3(__camera2world_matrix * vec4(vec3(0.0f), 1.0f));
	}

	virtual ~BasicCamera() {}

	inline void setScene(const Scene *scene_ptr) {
		__scene_ptr = scene_ptr;
	}

	inline size_t getHeight(size_t width) const {
		return size_t(width / __radio);
	}
};

#endif