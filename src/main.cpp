#include <iostream>
#include <vector>

#include "models/Sphere.hpp"
#include "cameras/PinHoleCamera.hpp"
#include "scenes/scene.hpp"

#include <glm/glm.hpp>

using namespace glm;
using namespace std;

int main() {
	float radio = 4.0 / 3.0;
	unsigned int width = 640;
	vec3 background_color = vec3(0.5f);
	vec3 ambient_radio(0.1);

	Scene scene(background_color, ambient_radio);
	scene.models.push_back(new Sphere(vec4(0, 0, -20, 1), 4, vec3(1, 0, 0), 1.0, 0.5));
	scene.models.push_back(new Sphere(vec4(5.0, -1, -15, 1), 2, vec3(0.0, 1.0, 0.0), 0, 1));
	scene.models.push_back(new Sphere(vec4(5.0, 0, -25, 1), 3, vec3(0.0, 0.0, 1.0), 0, 1));
	scene.models.push_back(new Sphere(vec4(-5.5, 0, -15, 1), 3, vec3(0.0, 1.0, 0.0), 0, 1));
	scene.models.push_back(new Sphere(vec4(0, -10000, 0, 1), 9995, vec3(0.1), 0, 1));

	Sphere* light = new Sphere(vec4(0, 20, -30, 1), 3, vec3(0.0, 0.0, 0.0), 0, 0, vec3(3.0));
	scene.models.push_back(light);
	scene.lights.push_back(light);

	PinHoleCamera camera(vec4(vec3(0), 1), vec4(0, 0, -1, 0), vec4(0, 1, 0, 0), radians(45.0f), radio, 1);
	camera.setScene(&scene);

	Mat result(width / radio, width, CV_32FC3);
	camera.render(width, 1, result);

	return 0;
}