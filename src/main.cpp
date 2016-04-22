#include <opencv2/highgui/highgui.hpp>

#include "common.hpp"

#include "models/Sphere.hpp"
#include "models/Plane.hpp"
#include "models/Mesh.hpp"
#include "models/Cube.hpp"

#include "cameras/PinHoleCamera.hpp"
#include "scenes/scene.hpp"

int main() {
	float radio = 4.0 / 3.0;
	unsigned int width = 640;

	Scene scene(vec3(0), 0, 0.15, 5, 5);

	//                                                              color, diffuse, specular, specular_power, reflection, reflect_radio, transparency, refraction_radio, absorbance
	scene.addModel(new Plane (vec3(0, -1, 0), 7.4,					vec3(0.4, 0.7, 0.7), 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.00));
	scene.addModel(new Plane (vec3(0.4, 0, 1), 12,			 		vec3(0.5, 0.3, 0.5), 0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.00));
	scene.addModel(new Plane (vec3(0, 1, 0), 4.4,		 			vec3(0.4, 0.3, 0.3), 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.00));

	scene.addModel(new Sphere(vec3(2, 0.8, -3), 2.5, 				vec3(0.7, 0.7, 1.0), 0.0, 0.0, 0.0, 0.2, 0.1, 0.8, 1.3, 0.15));
	scene.addModel(new Sphere(vec3(-5.5, -0.5, -7), 2, 				vec3(0.7, 0.7, 1.0), 0.1, 0.0, 0.0, 0.5, 0.1, 0.0, 1.3, 0.00));
	scene.addModel(new Sphere(vec3(-1.5, -3.8, -1), 1.5, 			vec3(1.0, 0.4, 0.4), 0.1, 1.0, 20 , 0.0, 0.0, 0.8, 1.0, 0.15));
	scene.addModel(new Cube(vec3(-2, 0.8, -3), vec3(1.5), 			vec3(0.0, 1.0, 0.0), 1.0, 1.0, 20 , 0.0, 0.0, 0.0, 0.0, 0.00));
	scene.addModel(new Mesh("objs/cube.obj", 30, 					vec3(1, 0, 0), 1.0, 1.0, 20, 0.0, 0.0, 0.0, 0.0, 0.00));

	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 7; y++) {
			scene.addModel(new Sphere(vec3(-4.5 + x * 1.5, -4.3 + y * 1.5, -10), 0.3,
			                          vec3(0.3, 1.0, 0.4), 0.6, 0.6, 20, 0.0, 0.0, 0.0, 0.0, 0.0));
		}
	}

	scene.addLight(new Sphere(vec3(0.4), vec3(0, 5, -5), 0.1));
	scene.addLight(new Sphere(vec3(0.6, 0.6, 0.8), vec3(-3, 5, -1), 0.1));
	scene.addLight(new Sphere(vec3(1.0, 1.0, 1.0), vec3(-2, 2, 5), 0.1));

	PinHoleCamera camera(vec3(0, 0, 5), vec3(0, 0, -1), vec3(0, 1, 0), radians(60.0f), radio, 1);
	camera.setScene(&scene);

	Mat result(width / radio, width, CV_32FC3);
	camera.render(width, 1, result);
	imshow("Raytracing", result);
	waitKey();

	// vector<int> params;
	// params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	// params.push_back(0);
	// imwrite("Raytracing.png", result, params);

	return 0;
}