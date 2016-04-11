#include <iostream>
#include <vector>

#include "models/Sphere.hpp"
#include "cameras/PinHoleCamera.hpp"

#include <glm/glm.hpp>

using namespace glm;
using namespace std;

int main() {
	Sphere* sphere = new Sphere(vec3(1.0, 1.0, 1.0), 0.5, 0.5);
	PinHoleCamera camera(vec3(0,0,2), vec3(0, 0, -1), vec3(0, 1, 0), radians(135.0f), 4.0/3.0, 0.1);

	vector<BasicModel*> v;
	v.push_back(sphere);

	camera.render(v, 1024, 2);

	return 0;
}