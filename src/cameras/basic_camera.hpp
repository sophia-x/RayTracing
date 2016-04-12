#ifndef BASIC_CAMERA
#define BASIC_CAMERA

#include <glm/glm.hpp>
#include <vector>
#include <opencv2/opencv.hpp>

#include "../models/basic_model.hpp"
#include "../scenes/scene.hpp"

using namespace glm;
using namespace std;
using namespace cv;

class BasicCamera {
protected:
	vec4 position, direction, up;
	float fov, radio, im_dist;
	const Scene *scene_ptr;

public:
	BasicCamera(const vec4 &position, const vec4 &direction, const vec4 &up,
	            float fov, float radio, float im_dist): fov(fov), radio(radio), im_dist(im_dist), position(position), direction(direction), up(up) {}

	virtual ~BasicCamera() {}

	virtual void render(unsigned short width, unsigned short anti_t, Mat &result) const = 0;

	void setScene(const Scene *scene_p){
		scene_ptr = scene_p;
	}

protected:
	virtual vec3 raytracing(const vec4 &position, const vec4 &direction) const = 0;
};

#endif