#ifndef SCENE
#define SCENE

#include <vector>
#include "../models/basic_model.hpp"

using namespace std;

class Scene {
public:
	vector<BasicModel *> models;
	vector<BasicModel *> lights;
	vec3 backgroud_color;
	vec3 ambient_radio;

	Scene(const vec3 &backgroud_color, const vec3 &ambient_radio): backgroud_color(backgroud_color), ambient_radio(ambient_radio) {}
};

#endif