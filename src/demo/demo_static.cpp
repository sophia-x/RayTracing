#include "demo.hpp"

Mat scene_normal(float radio, size_t width) {
	/*************************Init Scene**********************/
	Scene scene(vec3(0), 5);

	/*************************Add textures**********************/
	Texture *chess_board = new Texture("textures/chess_board.jpg");
	Texture *mountain = new Texture("textures/mountain.jpg");
	Texture *planet = new Texture("textures/planet.jpg");
	scene.addTexture(chess_board);
	scene.addTexture(mountain);
	scene.addTexture(planet);

	/*************************Add models**********************/
	//background plane
	scene.addModel(Plane (vec3(13, 10, -15), vec3(-13, 10, -15), vec3(-13, -7.4, -15), vec3(13, -7.4, -15),
	                      vec2(1.0, 0.0), vec2(0.0, 0.0), vec2(0.0, 1.0), vec2(1.0, 1.0),
	                      Material(mountain, 1, 1, vec3(1.0), 0.5, 0.0, 00, 0.0, 0.0, 0.0, 0.00)));
	//ground plane
	scene.addModel(Plane (vec3(13, -2, -13), vec3(-13, -2, -13), vec3(-13, -2, 0), vec3(13, -2, 0),
	                      vec2(1.0, 0.0), vec2(0.0, 0.0), vec2(0.0, 1.0), vec2(1.0, 1.0),
	                      Material(chess_board, 1.0, 1.0, vec3(1.0), 0.5, 0.0, 00, 0.0, 0.0, 0.0, 0.00)));
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

	/*************************Init k-d tree**********************/
	scene.buildWorld();

	/*************************Init camera**********************/
	// PinHoleCamera
	PinHoleCamera camera(vec3(0, 0, 0), vec3(0, 0, -1), vec3(0, 1, 0), radians(60.0f), radio);
	camera.setScene(&scene);

	/*************************Start rendering**********************/
	Mat result(camera.getHeight(width), width, CV_32FC3);
	// PinHoleCamera
	camera.render(result, 1);

	return result;
}

Mat scene_area_light(float radio, size_t width) {
	/*************************Init Scene**********************/
	Scene scene(vec3(0), 5);

	/*************************Add textures**********************/
	Texture *chess_board = new Texture("textures/chess_board.jpg");
	Texture *mountain = new Texture("textures/mountain.jpg");
	Texture *planet = new Texture("textures/planet.jpg");
	scene.addTexture(chess_board);
	scene.addTexture(mountain);
	scene.addTexture(planet);

	/*************************Add models**********************/
	//background plane
	scene.addModel(Plane (vec3(13, 10, -15), vec3(-13, 10, -15), vec3(-13, -7.4, -15), vec3(13, -7.4, -15),
	                      vec2(1.0, 0.0), vec2(0.0, 0.0), vec2(0.0, 1.0), vec2(1.0, 1.0),
	                      Material(mountain, 1, 1, vec3(1.0), 0.5, 0.0, 00, 0.0, 0.0, 0.0, 0.00)));

	//ground plane
	scene.addModel(Plane (vec3(13, -2, -13), vec3(-13, -2, -13), vec3(-13, -2, 0), vec3(13, -2, 0),
	                      vec2(1.0, 0.0), vec2(0.0, 0.0), vec2(0.0, 1.0), vec2(1.0, 1.0),
	                      Material(chess_board, 1.0, 1.0, vec3(1.0), 0.5, 0.0, 00, 0.0, 0.0, 0.0, 0.00)));
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
	// Area Light
	scene.addLight(new AreaLight(vec3(-2, 3.5, 0), 1, 1, vec3(2.0)));

	/*************************Init k-d tree**********************/
	scene.buildWorld();

	/*************************Init camera**********************/
	// PinHoleCamera
	PinHoleCamera camera(vec3(0, 0, 0), vec3(0, 0, -1), vec3(0, 1, 0), radians(60.0f), radio);
	camera.setScene(&scene);

	/*************************Start rendering**********************/
	Mat result(camera.getHeight(width), width, CV_32FC3);
	// PinHoleCamera
	camera.render(result, 1);

	return result;
}

Mat scene_depth_camera(float radio, size_t width) {
	/*************************Init Scene**********************/
	Scene scene(vec3(0), 5);

	/*************************Add textures**********************/
	Texture *chess_board = new Texture("textures/chess_board.jpg");
	Texture *mountain = new Texture("textures/mountain.jpg");
	Texture *planet = new Texture("textures/planet.jpg");
	scene.addTexture(chess_board);
	scene.addTexture(mountain);
	scene.addTexture(planet);

	/*************************Add models**********************/
	//background plane
	scene.addModel(Plane (vec3(13, 10, -15), vec3(-13, 10, -15), vec3(-13, -7.4, -15), vec3(13, -7.4, -15),
	                      vec2(1.0, 0.0), vec2(0.0, 0.0), vec2(0.0, 1.0), vec2(1.0, 1.0),
	                      Material(mountain, 1, 1, vec3(1.0), 0.5, 0.0, 00, 0.0, 0.0, 0.0, 0.00)));
	//ground plane
	scene.addModel(Plane (vec3(13, -2, -13), vec3(-13, -2, -13), vec3(-13, -2, 0), vec3(13, -2, 0),
	                      vec2(1.0, 0.0), vec2(0.0, 0.0), vec2(0.0, 1.0), vec2(1.0, 1.0),
	                      Material(chess_board, 1.0, 1.0, vec3(1.0), 0.5, 0.0, 00, 0.0, 0.0, 0.0, 0.00)));
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

	/*************************Init k-d tree**********************/
	scene.buildWorld();

	/*************************Init camera**********************/
	// Depth Camera
	DepthCamera camera(vec3(0, 0, 0), vec3(0, 0, -1), vec3(0, 1, 0), radians(60.0f), radio);
	camera.setScene(&scene);

	/*************************Start rendering**********************/
	Mat result(camera.getHeight(width), width, CV_32FC3);
	// Depth Camera
	camera.render(result, 5, 8, 16, 16);

	return result;
}

Mat scene_normal_texture(float radio, size_t width) {
	/*************************Init Scene**********************/
	Scene scene(vec3(0), 5);

	/*************************Add textures**********************/
	Texture *planet = new Texture("textures/planet.jpg");
	Texture *rock = new Texture("textures/rock.png", "textures/rock_normal.png", 0.5);
	Texture *water = new Texture("textures/normal.png", 0.1);
	scene.addTexture(planet);
	scene.addTexture(rock);
	scene.addTexture(water);

	/*************************Add models**********************/
	//background plane
	scene.addModel(Plane (vec3(13, 10, -15), vec3(-13, 10, -15), vec3(-13, -7.4, -15), vec3(13, -7.4, -15),
	                      vec2(1.0, 0.0), vec2(0.0, 0.0), vec2(0.0, 1.0), vec2(1.0, 1.0),
	                      Material(rock, 0.25, 0.25, vec3(1.0), 0.5, 0.0, 00, 0.0, 0.0, 0.0, 0.00)));
	//ground plane
	scene.addModel(Plane (vec3(13, -2, -13), vec3(-13, -2, -13), vec3(-13, -2, 0), vec3(13, -2, 0),
	                      vec2(1.0, 0.0), vec2(0.0, 0.0), vec2(0.0, 1.0), vec2(1.0, 1.0),
	                      Material(water, 1.0, 1.0, vec3(37, 243, 250) / 255.0f, 0.0, 0.0, 00, 1.0, 0.0, 0.0, 0.00)));
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

	/*************************Init k-d tree**********************/
	scene.buildWorld();

	/*************************Init camera**********************/
	// PinHoleCamera
	PinHoleCamera camera(vec3(0, 0, 0), vec3(0, 0, -1), vec3(0, 1, 0), radians(60.0f), radio);
	camera.setScene(&scene);

	/*************************Start rendering**********************/
	Mat result(camera.getHeight(width), width, CV_32FC3);
	// PinHoleCamera
	camera.render(result, 1);

	return result;
}