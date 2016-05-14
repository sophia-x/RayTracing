#include "demo.hpp"

#define __FPS__ 12

const static mat4 I = mat4(1);

class KnotSpirit: public Spirit {
private:
	Mesh *__knot;
	mat4 __base;
	float __rotate_speed;
	float __min_scale, __max_scale;
	float __scale_speed;
	float __scale;

public:
	KnotSpirit(Mesh *knot, const vec3 &position, float size): __knot(knot), __base(translate(position) * scale(vec3(size))) {
		__rotate_speed = PI / 2;
		__scale = __min_scale = 0.5; __max_scale = 1.5;
		__scale_speed = 0.3;
	}

	void update(size_t idx, size_t FPS) {
		if (__scale > __max_scale || __scale < __min_scale) {
			__scale_speed *= -1;
		}
		__scale += __scale_speed / FPS;

		__knot->transform(__base * scale(vec3(__scale)) * rotate((__rotate_speed * idx) / FPS, vec3(1, 1, 1)));
	}
};

class SetSpirit: public Spirit {
private:
	ModelSet *__set;
	float __rotate_speed1, __rotate_speed2, __rotate_speed3;

public:
	SetSpirit(ModelSet *set): __set(set) {
		__rotate_speed1 = PI / 2;
		__rotate_speed2 = PI / 1.5;
		__rotate_speed3 = PI / 4;
	}

	void update(size_t idx, size_t FPS) {
		__set->setMat(0, rotate((__rotate_speed1 * idx) / FPS, vec3(1, 0, 0))*translate(vec3(0, 1.2, 0))*scale(vec3(0.25)));
		__set->setMat(1, rotate((__rotate_speed2 * idx) / FPS, vec3(0, 1, 0))*translate(vec3(1.5, 0, 0))*scale(vec3(0.35)));
		__set->setMat(2, rotate((__rotate_speed3 * idx) / FPS, vec3(0, 1, 0))*scale(vec3(0.5)));

		__set->transform(translate(vec3(0.5, 0.5, -4)));
	}
};

class PointLightSpirit : public Spirit {
private:
	PointLight *__light;
	float __rotate_speed;

public:
	PointLightSpirit(PointLight *light): __light(light) {
		__rotate_speed = PI / 8;
	}

	void update(size_t idx, size_t FPS) {
		__light->transform(rotate((__rotate_speed * idx) / FPS, vec3(0, 0, -1))*translate(vec3(-5, 0, 0))*scale(vec3(0.01)));
	}
};

void scene_animation(const char *file_name, size_t width, float radio) {
	/*************************Init Scene**********************/
	Scene scene(vec3(0), 5);

	/*************************Add textures**********************/
	Texture *chess_board = new Texture("textures/chess_board.jpg");
	Texture *mountain = new Texture("textures/mountain.jpg");
	Texture *planet = new Texture("textures/planet.jpg");
	scene.addTexture(chess_board);
	scene.addTexture(mountain);
	scene.addTexture(planet);

	/*************************Spirits vector**********************/
	vector<Spirit *> spirits;

	/*************************Add models & Spirits**********************/
	// background plane
	scene.addModel(Plane (vec3(13, 10, -15), vec3(-13, 10, -15), vec3(-13, -7.4, -15), vec3(13, -7.4, -15),
	                      vec2(1.0, 0.0), vec2(0.0, 0.0), vec2(0.0, 1.0), vec2(1.0, 1.0),
	                      Material(mountain, 1, 1, vec3(1.0), 0.5, 0.0, 00, 0.0, 0.0, 0.0, 0.00)));
	// ground plane
	scene.addModel(Plane (vec3(13, -2, -13), vec3(-13, -2, -13), vec3(-13, -2, 0), vec3(13, -2, 0),
	                      vec2(1.0, 0.0), vec2(0.0, 0.0), vec2(0.0, 1.0), vec2(1.0, 1.0),
	                      Material(chess_board, 1.0, 1.0, vec3(1.0), 0.5, 0.0, 00, 0.0, 0.0, 0.0, 0.00)));

	// Spheres
	SphereModel *sphere1 = new SphereModel(Material(vec3(0.0, 1.0, 0.0), 0.1, 0.0, 00, 0.7, 0.0, 0.0, 0.00));
	SphereModel *sphere2 = new SphereModel(Material(vec3(1.0, 1.0, 1.0), 0.0, 0.0, 00, 0.0, 1.0, 1.3, 0.00));
	SphereModel *center = new SphereModel(Material(planet, 1.0, 1.0, vec3(1.0, 1.0, 1.0), 0.8, 0.2, 20, 0.0, 0.0, 0.0, 0.00));
	vector<Model *> models = {sphere1, sphere2, center};
	vector<mat4> mats = {I, I, I};
	ModelSet *set = new ModelSet(models, mats);
	scene.addModel(set);
	spirits.push_back(new SetSpirit(set));

	//OBJ
	Mesh *knot = new Mesh("objs/knot.3DS", Material(vec3(1.0, 0.0, 0.0), 0.5, 0.5, 20, 0.0, 0.0, 0.0, 0.00));
	scene.addModel(knot);
	spirits.push_back(new KnotSpirit(knot, vec3(-1, 0, -2.5), 0.5));

	/*************************Add Light**********************/
	// Point Light
	PointLight *p_light = new PointLight(vec3(1.5));
	scene.addLight(p_light);
	spirits.push_back(new PointLightSpirit(p_light));

	// AreaLight
	scene.addLight(new AreaLight(vec3(0, 5, -4), 1, 1, vec3(0.35)));

	/*************************Init camera**********************/
	// PinHoleCamera
	// PinHoleCamera camera(vec3(0, 0, 0), vec3(0, 0, -1), vec3(0, 1, 0), radians(60.0f), radio);

	// DepthCamera
	DepthCamera camera(vec3(0, 0, 0), vec3(0, 0, -1), vec3(0, 1, 0), radians(60.0f), radio);

	camera.setScene(&scene);

	/*************************Init VideoWriter*********************/
	VideoWriter writer(file_name, CV_FOURCC('m', 'p', '4', 'v'), __FPS__, Size(width, camera.getHeight(width)));

	/*************************Animation generation**********************/
	float f = 1000;
	float f_s = 1.1;

	for (size_t i = 0; i < __FPS__ * 10; i++) {
		cout << "*******************************************************************" << endl;
		cout << "Frame " << i << endl;
		auto begin = chrono::system_clock::now();

		/*************************Update spirits' states**********************/
		for (auto it = spirits.begin(); it != spirits.end(); it++)
			(*it)->update(i, __FPS__);

		/*************************Init k-d tree**********************/
		scene.buildWorld();

		/*************************Start rendering**********************/
		// DepthCamera
		Mat result(camera.getHeight(width), width, CV_32FC3);
		{
			float min_f = 1, max_f = 1000;
			if (f > max_f || f < min_f)
				f_s = 1.0 / f_s;
			f /= f_s;
			camera.render(result, f, 4, 16, 16);

			// camera.render(result, 4);
		}

		auto end = chrono::system_clock::now();
		std::chrono::duration<double> dur = end - begin;
		cout << "Frame " << i << " generation time:" << dur.count() << " s" << endl;

		/*************************Write & Show**************************/
		imshow("Animation", result);
		waitKey(1);
		writer << result;
	}

	for (auto it = spirits.begin(); it != spirits.end(); it++)
		delete (*it);
}