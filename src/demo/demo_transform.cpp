#include "demo.hpp"

Mat scene_transform_demo(float radio, size_t width) {
	/*************************Init Scene**********************/
	Scene scene(vec3(0), 5);

	/*************************Add textures**********************/
	Texture *planet = new Texture("textures/planet.jpg");
	Texture *chess_board = new Texture("textures/chess_board.jpg");
	scene.addTexture(planet);
	scene.addTexture(chess_board);

	/*************************Add models**********************/
	// Sphere
	SphereModel *sphere = new SphereModel(Material(planet, 1.0, 1.0, vec3(1.0, 1.0, 1.0), 0.8, 0.2, 20, 0.0, 0.0, 0.0, 0.00));
	scene.addModel(sphere);
	mat4 sphere_m = translate(vec3(0, 1, -1)) * rotate(PI / 2, vec3(1, 0, 0)) * scale(vec3(1.0));
	sphere->transform(sphere_m);

	// Plane
	Mesh *plane = Plane (vec2(1.0, 0.0), vec2(0.0, 0.0), vec2(0.0, 1.0), vec2(1.0, 1.0),
	                     Material(chess_board, 1.0, 1.0, vec3(1.0), 0.5, 0.0, 00, 0.0, 0.0, 0.0, 0.00));
	scene.addModel(plane);
	mat4 plane_m = translate(vec3(0, -1, 0)) * rotate(PI / 4, vec3(0, 1, 0)) * scale(vec3(5, 0, 10));
	plane->transform(plane_m);

	// Box
	Mesh *box = Cube(Material(vec3(0.0, 1.0, 0.0), 0.1, 1.0, 20, 0.0, 0.0, 0.0, 0.00));
	scene.addModel(box);
	mat4 box_m = translate(vec3(0, -1, 0)) * rotate(PI / 4, vec3(0, 1, 0)) * scale(vec3(0.5));
	box->transform(box_m);

	// OBJ
	Mesh *knot = new Mesh("objs/knot.3DS", Material(vec3(1.0, 0.0, 0.0), 0.5, 0.5, 20, 0.0, 0.0, 0.0, 0.00));
	scene.addModel(knot);
	mat4 knot_m = translate(vec3(-2, 0, 0)) * rotate(PI / 4, vec3(0, 1, 0)) * scale(vec3(0.5));
	knot->transform(knot_m);

	// ModelSet
	SphereModel *sphere1 = new SphereModel(Material(planet, 1.0, 1.0, vec3(1.0, 0.0, 0.0), 0.8, 0.2, 20, 0.0, 0.0, 0.0, 0.00));
	SphereModel *sphere2 = new SphereModel(Material(planet, 1.0, 1.0, vec3(0.0, 1.0, 0.0), 0.8, 0.2, 20, 0.0, 0.0, 0.0, 0.00));
	vector<Model *> models; models.push_back(sphere1); models.push_back(sphere2);
	vector<mat4> mats; mats.push_back(translate(vec3(-1, 0, 0))); mats.push_back(translate(vec3(1, 0, 0)));
	ModelSet *set = new ModelSet(models, mats);
	scene.addModel(set);
	mat4 set_m = translate(vec3(2, 0, 0)) * rotate(PI / 4, vec3(0, 1, 0)) * scale(vec3(0.5));
	set->transform(set_m);

	/*************************Add Light**********************/
	// Point_Light
	PointLight *point_light = new PointLight(vec3(2.0));
	scene.addLight(point_light);
	mat4 light_m = translate(vec3(5, 5, -1)) * scale(vec3(0.01));
	point_light->transform(light_m);

	// Area_Light
	AreaLight *area_light = new AreaLight(vec3(1.0));
	scene.addLight(area_light);
	mat4 a_light_m = translate(vec3(-5, 5, -1)) * rotate(PI / 4, vec3(1, 0, 0)) * scale(vec3(0.3));
	area_light->transform(a_light_m);

	/*************************Init k-d tree**********************/
	scene.buildWorld();

	/*************************Init camera**********************/
	// PinHole Camera
	PinHoleCamera camera(vec3(0, 0, 3), vec3(0, 0, -1), vec3(0, 1, 0), radians(60.0f), radio);
	camera.transform(vec3(0, 0, 5), vec3(0, 0.2, -1), vec3(0, 1, 0));

	// Depth Camera
	// DepthCamera camera(vec3(0, 0, 0), vec3(0, 0, -1), vec3(0, 1, 0), radians(60.0f), radio);
	// camera.transform(vec3(0, 0, 5), vec3(0, 0.2, -1), vec3(0, 1, 0));

	camera.setScene(&scene);
	/*************************Start rendering**********************/
	Mat result(camera.getHeight(width), width, CV_32FC3);

	camera.render(result, 1);
	// camera.render(result, 5, 8, 16, 16);

	return result;
}