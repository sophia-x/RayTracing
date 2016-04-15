#include <iostream>
#include <vector>
#include <opencv2/highgui/highgui.hpp>

#include "models/Sphere.hpp"
#include "cameras/PinHoleCamera.hpp"
#include "scenes/scene.hpp"

#include <glm/glm.hpp>

using namespace glm;
using namespace std;

int main() {
	vec3 background_color = vec3(2.0f);
	float ambient_radio = 0.05, transparency_power = 0.35, fres_power = 5;
	unsigned char max_recursive = 4;

	float radio = 4.0 / 3.0;
	unsigned int width = 640;

	float specular_power = 15;
	float absorbance = 0.05;

	Scene scene(background_color, ambient_radio, transparency_power, fres_power, max_recursive);
	// center, radius, surfaceColor, diffuse, specular, specular_power, reflection, reflect_radio, transparency, refraction_radio, absorbance
	scene.addModel(new Sphere(vec4(0, 0, -20, 1),		4, 		vec3(1, 0.0, 0.0), 		0.0, 0.0, specular_power, 1.0, 0.1, 0.5,  1.1, absorbance));
	scene.addModel(new Sphere(vec4(5.0, -1, -15, 1),	2, 		vec3(0.0, 1.0, 0.0),	0.0, 0.0, specular_power, 1.0, 0.1, 0.0,  0.0, absorbance));
	scene.addModel(new Sphere(vec4(5.0, 0, -25, 1), 	3, 		vec3(0.0, 0.0, 1.0), 	0.5, 0.3, specular_power, 0.0, 0.0, 0.0,  0.0, absorbance));
	scene.addModel(new Sphere(vec4(-5.5, 0, -15, 1), 	3, 		vec3(0.0, 1.0, 0.0), 	0.5, 0.3, specular_power, 0.0, 0.0, 0.0,  0.0, absorbance));
	scene.addModel(new Sphere(vec4(0, -10004, -20, 1), 	10000, 	vec3(0.20), 			1.0, 0.0, specular_power, 0.0, 0.0, 0.0,  0.0, absorbance));

	scene.addLight(new Sphere(vec3(3), vec4(0, 20, -30, 1), 3));

	PinHoleCamera camera(vec4(vec3(0), 1), vec4(0, 0, -1, 0), vec4(0, 1, 0, 0), radians(30.0f), radio, 1);
	camera.setScene(&scene);

	Mat result(width / radio, width, CV_32FC3);
	camera.render(width, 2, result);
	imwrite("Raytracing.jpeg", result);
	
	return 0;
}