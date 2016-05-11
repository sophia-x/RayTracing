#ifndef SCENE
#define SCENE

#include <chrono>

#include "../common.hpp"
#include "../models/Model.hpp"
#include "../models/Texture.hpp"
#include "../models/Light.hpp"
#include "../models/Primitive.hpp"
#include "../utils/KD_Tree.hpp"

class Scene {
private:
	vector<Primitive *> __primitives;
	vector<const Model *> __models;
	vector<const Light *> __lights;
	vector<const Texture *> __textures;
	vec3 __backgroud_color;
	unsigned char __max_recursive;

	KD_Tree __tree;

public:
	Scene(const vec3 &backgroud_color, unsigned char max_recursive): __backgroud_color(backgroud_color), __max_recursive(max_recursive) {}

	~Scene() {
		for (auto it = __models.begin(); it != __models.end(); ++it) {
			delete (*it);
		}

		for (auto it = __textures.begin(); it != __textures.end(); ++it) {
			delete (*it);
		}
	}

	inline bool intersect(const Ray &ray, float &t, Primitive * &hit_ptr) const {
		return __tree.intersect(ray, t, hit_ptr);
	}

	inline bool intersect(const Ray &ray, float len, unsigned long hash_code) const {
		return __tree.intersect(ray, len, hash_code);
	}

	inline void buildWorld() {
		cout << "total primitives:" << __primitives.size() << endl;

		auto begin = chrono::system_clock::now();

		__tree = KD_Tree(__primitives);

		auto end = chrono::system_clock::now();
		std::chrono::duration<double> dur = end - begin;
		cout << "Build KD Tree:" << dur.count() << " s" << endl;
	}

	inline void addModel(Model *model) {
		__models.push_back(model);
		model->addPrimitives(__primitives);
	}

	inline void addLight(Light *light) {
		__lights.push_back(light);
		addModel(light);
	}

	inline void addTexture(Texture *texture) {
		__textures.push_back(texture);
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