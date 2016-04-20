#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include <glm/glm.hpp>

#include "models/Sphere.hpp"
#include "models/Plane.hpp"
#include "models/Mesh.hpp"
#include "models/Cube.hpp"

#include "cameras/PinHoleCamera.hpp"
#include "scenes/scene.hpp"

#include <iostream>
#include <glm/gtx/string_cast.hpp>

using namespace glm;
using namespace std;

int main() {
	size_t threshold = 50;

	vec3 background_color = vec3(2.0f);
	float ambient_radio = 0.05, transparency_power = 0.35, fres_power = 5;
	unsigned char max_recursive = 4;

	float radio = 4.0 / 3.0;
	// unsigned int width = 320;
	unsigned int width = 320;

	float specular_power = 15;
	float absorbance = 0.05;

	Scene scene(background_color, ambient_radio, transparency_power, fres_power, max_recursive);
	// center, radius, surfaceColor, diffuse, specular, specular_power, reflection, reflect_radio, transparency, refraction_radio, absorbance
	// scene.addModel(new Sphere(vec4(0, 0, -20, 1),		4, 		vec3(1, 0.0, 0.0), 		0.0, 0.0, specular_power, 1.0, 0.1, 0.5,  1.1, absorbance));
	// scene.addModel(new Sphere(vec4(5.0, -1, -15, 1),	2, 		vec3(0.0, 1.0, 0.0),	0.0, 0.0, specular_power, 1.0, 0.1, 0.0,  0.0, absorbance));
	// scene.addModel(new Sphere(vec4(5.0, 0, -25, 1), 	3, 		vec3(0.0, 0.0, 1.0), 	0.5, 0.3, specular_power, 0.0, 0.0, 0.0,  0.0, absorbance));
	// scene.addModel(new Sphere(vec4(-5.5, 0, -15, 1), 	3, 		vec3(0.0, 1.0, 0.0), 	0.5, 0.3, specular_power, 0.0, 0.0, 0.0,  0.0, absorbance));
	// scene.addModel(new Plane (normalize(vec4(0, 1, 0, 0)), -4, 80, 80, vec3(0.2), 		1.0, 0.0, specular_power, 0.0, 0.0, 0.0,  0.0, absorbance));
	// scene.addModel(new Cube  (vec4(-3.5, 4, -20, 0),  vec3(1.5),   vec3(0.0, 1.0, 0.0), 0.5, 0.3, specular_power, 0.0, 0.0, 0.0,  0.0, absorbance));
	scene.addModel(new Mesh  ("objs/Arma.obj", threshold, vec3(0.45),					0.5, 0.3, specular_power, 	0.0, 0.0, 0.0,  0.0, absorbance));

	// scene.addLight(new Sphere(vec3(3), vec4(15, 25, -25, 1), 3));
	scene.addLight(new Sphere(vec3(3), vec4(5, 20, 0, 1), 3));

	// PinHoleCamera camera(vec4(vec3(0), 1), vec4(0, 0, -1, 0), vec4(0, 1, 0, 0), radians(30.0f), radio, 1);
	PinHoleCamera camera(vec4(vec3(0, 0, 2), 1), vec4(0, 0, -1, 0), vec4(0, 1, 0, 0), radians(30.0f), radio, 1);
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