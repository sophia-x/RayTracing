#ifndef BASIC_CAMERA
#define BASIC_CAMERA

#include <glm/glm.hpp>
#include <vector>

#include "../models/basic_model.hpp"

using namespace glm;
using namespace std;

class BasicCamera{
protected:
	vec3 position, direction, up;
	float fov, radio, im_dist;

public:
	BasicCamera(const vec3 &position, const vec3 &direction, const vec3 &up,
				float fov, float radio, float im_dist):fov(fov), radio(radio), im_dist(im_dist), position(position), direction(direction), up(up){}

	virtual ~BasicCamera(){}

	virtual void render(const vector<BasicModel*> &models, unsigned short width, unsigned short anti_t) const = 0;

protected:
	virtual vec3 raytracing(const vec3 &position, const vec3 &direction, const vector<BasicModel*> &models, const vector<unsigned int> &lights_idx) const = 0;
};

#endif