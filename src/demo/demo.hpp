#ifndef DEMO
#define DEMO

#include <opencv2/highgui/highgui.hpp>
#include <chrono>

#include <glm/gtx/transform.hpp>

#include "../common.hpp"

#include "../models/Material.hpp"
#include "../models/Model.hpp"
#include "../models/PointLight.hpp"
#include "../models/AreaLight.hpp"
#include "../models/Texture.hpp"
#include "../models/SphereModel.hpp"
#include "../models/Mesh.hpp"
#include "../models/model_util.hpp"
#include "../models/ModelSet.hpp"

#include "../cameras/PinHoleCamera.hpp"
#include "../cameras/DepthCamera.hpp"
#include "../scenes/scene.hpp"

class Spirit {
public:
	Spirit() {}
	virtual ~Spirit() {}
	virtual void update(size_t idx, size_t FPS = 24) = 0;
};

Mat scene_normal(float radio = 4.0 / 3.0, size_t width = 640);
Mat scene_area_light(float radio = 4.0 / 3.0, size_t width = 640);
Mat scene_depth_camera(float radio = 4.0 / 3.0, size_t width = 640);
Mat scene_normal_texture(float radio = 4.0 / 3.0, size_t width = 640);


Mat scene_transform_demo(float radio = 4.0 / 3.0, size_t width = 640);

void scene_animation(const char *file_name, size_t width = 640, float radio = 4.0 / 3.0);

#endif