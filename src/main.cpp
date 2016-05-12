#include <opencv2/highgui/highgui.hpp>
#include <chrono>

#include <glm/gtx/transform.hpp>

#include "common.hpp"

#include "models/Material.hpp"
#include "models/PointLight.hpp"
#include "models/AreaLight.hpp"
#include "models/Texture.hpp"
#include "models/SphereModel.hpp"
#include "models/Mesh.hpp"
#include "models/model_util.hpp"
#include "models/ModelSet.hpp"

#include "cameras/PinHoleCamera.hpp"
#include "cameras/DepthCamera.hpp"
#include "scenes/scene.hpp"

Mat scene_simple();
Mat scene_obj();
Mat scene_motion();

int main() {
	//                                                              color, diffuse, specular, specular_power, reflection, transparency, refraction_radio, absorbance
	// scene.addModel(Plane (vec3(13, 7.4, 5.5), vec3(-13, 7.4, 5.5), vec3(-13, 7.4, -29),
	//                       vec3(13, 7.4, -29), 						Material(vec3(0.4, 0.7, 0.7), 0.5, 0.0, 00, 0.0, 0.0, 0.0, 0.00)));

	// scene.addModel(Plane (vec3(-13, -4.4, -8), vec3(13, -4.4, -16), vec3(13, 7.4, -16),
	//                       vec3(-13, 7.4, -8), 			 			Material(vec3(0.5, 0.3, 0.5), 0.6, 0.0, 00, 0.0, 0.0, 0.0, 0.00)));

	// scene.addModel(Plane (vec3(-13, -4.4, 5.5), vec3(13, -4.4, 5.5), vec3(13, -4.4, -29),
	//                       vec3( -13, -4.4, -29),					Material(vec3(0.4, 0.3, 0.3), 1.0, 0.0, 00, 0.0, 0.0, 0.0, 0.00)));

	// scene.addModel(new SphereModel(vec3(2, 0.8, -3), 2.5, 			Material(vec3(0.7, 0.7, 1.0), 0.0, 0.0, 00, 0.2, 0.8, 1.3, 0.15)));
	// scene.addModel(new SphereModel(vec3(-5.5, -0.5, -7), 2, 			Material(vec3(0.7, 0.7, 1.0), 0.1, 0.0, 00, 0.5, 0.0, 0.0, 0.00)));
	// scene.addModel(new SphereModel(vec3(-1.5, -3.8, -1), 1.5, 		Material(vec3(1.0, 0.4, 0.4), 0.1, 1.0, 20, 0.0, 0.3, 1.1, 0.15)));
	// scene.addModel(Cube(vec3(-2, 0.8, -3), vec3(1.5), 				Material(vec3(0.0, 1.0, 0.0), 1.0, 1.0, 20, 0.0, 0.0, 0.0, 0.00)));
	// scene.addModel(new Mesh("objs/cube.obj", 						Material(vec3(1.0, 0.0, 0.0), 1.0, 1.0, 20, 0.0, 0.0, 0.0, 0.00)));

	// for (int x = 0; x < 8; x++) {
	// 	for (int y = 0; y < 7; y++) {
	// 		scene.addModel(new SphereModel(vec3(-4.5 + x * 1.5, -4.3 + y * 1.5, -10),
	// 		                               0.3,						Material(vec3(0.3, 1.0, 0.4), 0.6, 0.6, 20, 0.0, 0.0, 0.0, 0.0)));
	// 	}
	// }

	// scene.addLight(new PointLight(vec3( 0, 5, -5), 0.1, vec3(0.4, 0.4, 0.4)));
	// scene.addLight(new PointLight(vec3(-3, 5, -1), 0.1, vec3(0.6, 0.6, 0.8)));
	// scene.addLight(new PointLight(vec3(-2, 2,  5), 0.1, vec3(1.0, 1.0, 1.0)));



	// PinHoleCamera camera(vec3(0, 0, 5), vec3(0, 0, -1), vec3(0, 1, 0), radians(60.0f), radio);
	// DepthCamera camera(vec3(0, 0, 5), vec3(0, 0, -1), vec3(0, 1, 0), radians(60.0f), radio);


	// camera.render(result, 15, 16, 40, 16);
	// camera.render(result, 4, 16, 1, 1);

	auto begin = chrono::system_clock::now();

	Mat result = scene_simple();
	// Mat result = scene_obj();
	// Mat result = scene_motion();

	auto end = chrono::system_clock::now();
	std::chrono::duration<double> dur = end - begin;
	cout << "KD Tree Builing + Raytracing:" << dur.count() << " s" << endl;

	imshow("Raytracing", result);

	vector<int> params;
	params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	params.push_back(0);
	imwrite("Raytracing.png", result, params);

	waitKey();
	return 0;
}

Mat scene_simple() {
	/*************************Default params**********************/
	float radio = 4.0 / 3.0;
	size_t width = 640;

	/*************************Init Scene**********************/
	Scene scene(vec3(0), 5);

	/*************************Add textures**********************/
	Texture *chess_board = new Texture("textures/chess_board.jpg");
	Texture *mountain = new Texture("textures/mountain.jpg");
	Texture *planet = new Texture("textures/planet.jpg");
	Texture *rock = new Texture("textures/rock.png", "textures/rock_normal.png", 0.5);
	Texture *water = new Texture("textures/normal.png", 0.1);
	scene.addTexture(chess_board);
	scene.addTexture(mountain);
	scene.addTexture(planet);
	scene.addTexture(rock);
	scene.addTexture(water);

	/*************************Add models**********************/

	//background plane

	scene.addModel(Plane (vec3(13, 10, -15), vec3(-13, 10, -15), vec3(-13, -7.4, -15), vec3(13, -7.4, -15),
	                      vec2(1.0, 0.0), vec2(0.0, 0.0), vec2(0.0, 1.0), vec2(1.0, 1.0),
	                      Material(mountain, 1, 1, vec3(1.0), 0.5, 0.0, 00, 0.0, 0.0, 0.0, 0.00)));
	// scene.addModel(Plane (vec3(13, 10, -15), vec3(-13, 10, -15), vec3(-13, -7.4, -15), vec3(13, -7.4, -15),
	//                       vec2(1.0, 0.0), vec2(0.0, 0.0), vec2(0.0, 1.0), vec2(1.0, 1.0),
	//                       Material(rock, 0.25, 0.25, vec3(1.0), 0.5, 0.0, 00, 0.0, 0.0, 0.0, 0.00)));

	//ground plane

	scene.addModel(Plane (vec3(13, -2, -13), vec3(-13, -2, -13), vec3(-13, -2, 0), vec3(13, -2, 0),
	                      vec2(1.0, 0.0), vec2(0.0, 0.0), vec2(0.0, 1.0), vec2(1.0, 1.0),
	                      Material(chess_board, 1.0, 1.0, vec3(1.0), 0.5, 0.0, 00, 0.0, 0.0, 0.0, 0.00)));

	// scene.addModel(Plane (vec3(13, -2, -13), vec3(-13, -2, -13), vec3(-13, -2, 0), vec3(13, -2, 0),
	//                       vec2(1.0, 0.0), vec2(0.0, 0.0), vec2(0.0, 1.0), vec2(1.0, 1.0),
	//                       Material(water, 1.0, 1.0, vec3(37, 243, 250) / 255.0f, 0.0, 0.0, 00, 1.0, 0.0, 0.0, 0.00)));

	// Spheres
	scene.addModel(new SphereModel(vec3(0.0, 1.0, -4), 0.5, 			Material(vec3(0.0, 1.0, 0.0), 0.1, 0.0, 00, 0.7, 0.0, 0.0, 0.00)));
	scene.addModel(new SphereModel(vec3(1.3, 0.0, -3), 0.5, 			Material(vec3(1.0, 1.0, 1.0), 0.0, 0.0, 00, 0.0, 1.0, 1.3, 0.00)));
	scene.addModel(new SphereModel(vec3(-2.0, 0.0, -5), 1.0, 			Material(planet, 1.0, 1.0, vec3(1.0, 1.0, 1.0), 0.8, 0.2, 20, 0.0, 0.0, 0.0, 0.00)));

	//OBJ
	Mesh *knot = new Mesh("objs/knot.3DS", Material(vec3(1.0, 0.0, 0.0), 0.5, 0.5, 20, 0.0, 0.0, 0.0, 0.00));
	scene.addModel(knot);
	mat4 t_m = translate(vec3(0, 0, -2.5)) * scale(vec3(0.5));
	knot->transform(t_m);

	/*************************Add Light**********************/
	// Point Light
	scene.addLight(new PointLight(vec3(0, 3.5, 0), 0.1, vec3(2.0)));

	// Area Light
	// scene.addLight(new AreaLight(vec3(-2, 3.5, 0), 1, 1, vec3(2.0)));

	/*************************Init k-d tree**********************/
	scene.buildWorld();

	/*************************Init camera**********************/
	// PinHoleCamera
	PinHoleCamera camera(vec3(0, 0, 0), vec3(0, 0, -1), vec3(0, 1, 0), radians(60.0f), radio);

	// Depth Camera
	// DepthCamera camera(vec3(0, 0, 0), vec3(0, 0, -1), vec3(0, 1, 0), radians(60.0f), radio);

	camera.setScene(&scene);
	/*************************Start rendering**********************/
	Mat result(camera.getHeight(width), width, CV_32FC3);
	// PinHoleCamera
	camera.render(result, 1);

	//Depth Camera
	// camera.render(result, 5, 8, 16, 16);

	return result;
}

Mat scene_obj() {
	/*************************Default params**********************/
	float radio = 4.0 / 3.0;
	size_t width = 640;

	/*************************Init Scene**********************/
	Scene scene(vec3(0), 5);

	/*************************Add models**********************/

	scene.addModel(new Mesh("objs/knot.3DS", Material(vec3(1.0, 0.0, 0.0), 0.5, 0.5, 20, 0.0, 0.0, 0.0, 0.00)));
	// scene.addModel(new Mesh("objs/dinosaur.2k.obj", Material(vec3(1.0, 0.0, 0.0), 0.5, 0.5, 20, 0.0, 0.0, 0.0, 0.00)));

	/*************************Add Light**********************/
	// Point Light
	scene.addLight(new PointLight(vec3(-500, 500,  500), 0.1, vec3(1.0)));
	scene.addLight(new PointLight(vec3(500, 500,  500), 0.1, vec3(1.0)));
	scene.addLight(new PointLight(vec3(0, 500,  500), 0.1, vec3(1.0)));
	scene.addLight(new PointLight(vec3(0, 500,  -500), 0.1, vec3(1.0)));
	scene.addLight(new PointLight(vec3(500, 500,  -500), 0.1, vec3(1.0)));
	scene.addLight(new PointLight(vec3(-500, 500,  -500), 0.1, vec3(1.0)));

	scene.addLight(new PointLight(vec3(-500, -500,  500), 0.1, vec3(1.0)));
	scene.addLight(new PointLight(vec3(500, -500,  500), 0.1, vec3(1.0)));
	scene.addLight(new PointLight(vec3(0, -500,  500), 0.1, vec3(1.0)));
	scene.addLight(new PointLight(vec3(0, -500,  -500), 0.1, vec3(1.0)));
	scene.addLight(new PointLight(vec3(500, -500,  -500), 0.1, vec3(1.0)));
	scene.addLight(new PointLight(vec3(-500, -500,  -500), 0.1, vec3(1.0)));

	/*************************Init k-d tree**********************/
	scene.buildWorld();

	/*************************Init camera**********************/
	// PinHoleCamera
	PinHoleCamera camera(vec3(0, 0, 300), vec3(0, 0, -1), vec3(0, 1, 0), radians(60.0f), radio);

	camera.setScene(&scene);
	/*************************Start rendering**********************/
	Mat result(camera.getHeight(width), width, CV_32FC3);
	// PinHoleCamera
	camera.render(result, 1);

	return result;
}

Mat scene_motion() {
	const mat4 I = mat4(1);

	/*************************Default params**********************/
	float radio = 4.0 / 3.0;
	size_t width = 640;

	/*************************Init Scene**********************/
	Scene scene(vec3(0), 5);

	/*************************Add textures**********************/
	Texture *planet = new Texture("textures/planet.jpg");
	Texture *chess_board = new Texture("textures/chess_board.jpg");
	scene.addTexture(planet);
	scene.addTexture(chess_board);

	/*************************Add models**********************/
	// SphereModel *sphere = new SphereModel(Material(planet, 1.0, 1.0, vec3(1.0, 1.0, 1.0), 0.8, 0.2, 20, 0.0, 0.0, 0.0, 0.00));
	// scene.addModel(sphere);
	// mat4 t_m = translate(vec3(0, -1, -1)) * rotate(PI / 2, vec3(1, 0, 0)) * scale(vec3(0.5));
	// sphere->transform(t_m);

	// Mesh *plane = Plane (vec2(1.0, 0.0), vec2(0.0, 0.0), vec2(0.0, 1.0), vec2(1.0, 1.0),
	//                      Material(chess_board, 1.0, 1.0, vec3(1.0), 0.5, 0.0, 00, 0.0, 0.0, 0.0, 0.00));
	// scene.addModel(plane);
	// mat4 t_m = translate(vec3(0, -1, 0)) * rotate(PI / 4, vec3(0, 1, 0)) * scale(vec3(5, 0, 10));
	// plane->transform(t_m);

	// Mesh *box = Cube(Material(vec3(0.0, 1.0, 0.0), 0.1, 1.0, 20, 0.0, 0.0, 0.0, 0.00));
	// scene.addModel(box);
	// mat4 t_m = translate(vec3(0, -1, 0)) * rotate(PI / 4, vec3(0, 1, 0)) * scale(vec3(0.5));
	// box->transform(t_m);

	// Mesh *knot = new Mesh("objs/knot.3DS", Material(vec3(1.0, 0.0, 0.0), 0.5, 0.5, 20, 0.0, 0.0, 0.0, 0.00));
	// scene.addModel(knot);
	// mat4 t_m = translate(vec3(2, 0, 0)) * rotate(PI / 4, vec3(0, 1, 0)) * scale(vec3(0.5));
	// knot->transform(t_m);

	// SphereModel *sphere1 = new SphereModel(Material(planet, 1.0, 1.0, vec3(1.0, 0.0, 0.0), 0.8, 0.2, 20, 0.0, 0.0, 0.0, 0.00));
	// SphereModel *sphere2 = new SphereModel(Material(planet, 1.0, 1.0, vec3(0.0, 1.0, 0.0), 0.8, 0.2, 20, 0.0, 0.0, 0.0, 0.00));
	// vector<Model *> models; models.push_back(sphere1); models.push_back(sphere2);
	// vector<mat4> mats; mats.push_back(translate(vec3(-1, 0, 0))); mats.push_back(translate(vec3(1, 0, 0)));
	// ModelSet *set = new ModelSet(models, mats);
	// scene.addModel(set);
	// mat4 t_m = translate(vec3(2, 0, 0)) * rotate(PI / 4, vec3(0, 1, 0)) * scale(vec3(0.5));
	// set->transform(t_m);

	/*************************Add Light**********************/
	scene.addLight(new PointLight(vec3(-5, 5, 5), 0.1, vec3(2.0)));
	scene.addLight(new PointLight(vec3(5, 5, 5), 0.1, vec3(2.0)));
	scene.addLight(new PointLight(vec3(0, 5, 5), 0.1, vec3(2.0)));

	/*************************Init k-d tree**********************/
	scene.buildWorld();

	/*************************Init camera**********************/
	PinHoleCamera camera(vec3(0, 0, 3), vec3(0, 0, -1), vec3(0, 1, 0), radians(60.0f), radio);

	camera.setScene(&scene);
	/*************************Start rendering**********************/
	Mat result(camera.getHeight(width), width, CV_32FC3);
	camera.render(result, 1);

	return result;
}