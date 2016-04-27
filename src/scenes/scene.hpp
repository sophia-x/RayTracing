#ifndef SCENE
#define SCENE

#include "../common.hpp"
#include "../models/Model.hpp"
#include "../models/Light.hpp"
#include "../models/Primitive.hpp"
#include "../utils/KD_Tree.hpp"

class Scene {
private:
	vector<const Primitive *> __primitives;
	vector<const Model *> __models;
	vector<const Light *> __lights;
	vec3 __backgroud_color;
	unsigned char __max_recursive;

	KD_Tree __tree;

public:
	Scene(const vec3 &backgroud_color, unsigned char max_recursive): __backgroud_color(backgroud_color), __max_recursive(max_recursive) {}

	~Scene() {
		for (auto it = __models.begin(); it != __models.end(); ++it) {
			delete (*it);
		}
	}

	inline bool intersect(const Ray &ray, float &t, vec3 &hit_normal, vec3 &hit_surface_color, Primitive const* &hit_ptr) const {
		return __tree.intersect(ray, t, hit_normal, hit_surface_color, hit_ptr);
	}

	inline bool intersect(const Ray &ray, float len, unsigned long hash_code) const {
		return __tree.intersect(ray, len, hash_code);
	}

	inline void buildWorld() {
		__tree = KD_Tree(__primitives);
	}

	inline void addModel(Model *model) {
		__models.push_back(model);
		model->addPrimitives(__primitives);
	}

	inline void addLight(Light *light) {
		__lights.push_back(light);
		addModel(light);
	}

	inline const vector<const Light *>& getLights() const {
		return __lights;
	}

	inline const vec3 &getBackgroundColor() const {
		return __backgroud_color;
	}

	inline unsigned char getMaxRecursive() const {
		return __max_recursive;
	}
};

#endif