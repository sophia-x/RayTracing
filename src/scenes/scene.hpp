#ifndef SCENE
#define SCENE

#include <vector>
#include "../models/basic_model.hpp"

using namespace std;

class Scene {
private:
	vector<BasicModel *> models;
	vector<BasicModel *> lights;
	vec3 backgroud_color;
	float ambient_radio, transparency_power, fres_power;
	unsigned char max_recursive;

public:
	Scene(const vec3 &backgroud_color, float ambient_radio, float transparency_power, float fres_power, unsigned char max_recursive):
		backgroud_color(backgroud_color), ambient_radio(ambient_radio), transparency_power(transparency_power), fres_power(fres_power),
		max_recursive(max_recursive) {}

	~Scene() {
		for (vector<BasicModel *>::iterator it = models.begin(); it != models.end(); ++it) {
			delete (*it);
		}
	}

	inline void addModel(BasicModel *model) {
		models.push_back(model);
	}

	inline void addLight(BasicModel *light) {
		models.push_back(light);
		lights.push_back(light);
	}

	inline const vector<BasicModel *>& getModels() const {
		return models;
	}

	inline const vector<BasicModel *>& getLights() const {
		return lights;
	}

	inline vec3 getBackgroundColor() const {
		return backgroud_color;
	}

	inline float getAmbientRadio() const {
		return ambient_radio;
	}

	inline float getTransparencyPower() const {
		return transparency_power;
	}

	inline float getFresPower() const {
		return fres_power;
	}

	inline unsigned char getMaxRecursive() const {
		return max_recursive;
	}
};

#endif