#ifndef RAY
#define RAY

#include "../common.hpp"

class Ray {
private:
	vec3 __position;
	vec3 __direction;
	vec3 __inv_direction;

public:
	Ray(const vec3 &position, const vec3 &direction): __position(position), __direction(direction), __inv_direction(1.0f / direction) {}

	inline const vec3& getPosition() const {
		return __position;
	}

	inline const vec3& getDirection() const {
		return __direction;
	}

	inline const vec3& getInvDirection() const {
		return __inv_direction;
	}
};

#endif