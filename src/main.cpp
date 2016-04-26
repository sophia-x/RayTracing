#include <opencv2/highgui/highgui.hpp>

#include "common.hpp"

#include "models/Material.hpp"
#include "models/Sphere.hpp"
#include "models/Mesh.hpp"
#include "models/ModelSet.hpp"
#include "models/model_util.hpp"

#include "cameras/PinHoleCamera.hpp"
#include "scenes/scene.hpp"

int main() {
	float radio = 4.0 / 3.0;
	size_t width = 640;

	Scene scene(vec3(0), 5);

	//                                                              color, diffuse, specular, specular_power, reflection, transparency, refraction_radio, absorbance
	vector<BasicModel *> models;

	models.push_back(Plane (vec3(13, 7.4, 5.5), vec3(-13, 7.4, 5.5), vec3(-13, 7.4, -29),
	                        vec3(13, 7.4, -29), 					vec3(0.4, 0.7, 0.7), Material(0.5, 0.0, 00, 0.0, 0.0, 0.0, 0.00)));

	models.push_back(Plane (vec3(-13, -4.4, -8), vec3(13, -4.4, -16), vec3(13, 7.4, -16),
	                        vec3(-13, 7.4, -8), 			 		vec3(0.5, 0.3, 0.5), Material(0.6, 0.0, 00, 0.0, 0.0, 0.0, 0.00)));

	models.push_back(Plane (vec3(-13, -4.4, 5.5), vec3(13, -4.4, 5.5), vec3(13, -4.4, -29),
	                        vec3( -13, -4.4, -29),					vec3(0.4, 0.3, 0.3), Material(1.0, 0.0, 00, 0.0, 0.0, 0.0, 0.00)));

	models.push_back(new Sphere(vec3(2, 0.8, -3), 2.5, 				vec3(0.7, 0.7, 1.0), Material(0.0, 0.0, 00, 0.2, 0.8, 1.3, 0.15)));
	models.push_back(new Sphere(vec3(-5.5, -0.5, -7), 2, 			vec3(0.7, 0.7, 1.0), Material(0.1, 0.0, 00, 0.5, 0.0, 1.3, 0.00)));
	models.push_back(new Sphere(vec3(-1.5, -3.8, -1), 1.5, 			vec3(1.0, 0.4, 0.4), Material(0.1, 1.0, 20, 0.0, 0.3, 1.1, 0.15)));
	models.push_back(Cube(vec3(-2, 0.8, -3), vec3(1.5), 			vec3(0.0, 1.0, 0.0), Material(1.0, 1.0, 20, 0.0, 0.0, 0.0, 0.00)));
	models.push_back(new Mesh("objs/cube.obj", 						vec3(1.0, 0.0, 0.0), Material(1.0, 1.0, 20, 0.0, 0.0, 0.0, 0.00)));

	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 7; y++) {
			models.push_back(new Sphere(vec3(-4.5 + x * 1.5, -4.3 + y * 1.5, -10),
			                            0.3,						vec3(0.3, 1.0, 0.4), Material(0.6, 0.6, 20, 0.0, 0.0, 0.0, 0.0)));
		}
	}
	scene.addModel(new ModelSet(models));

	scene.addLight(new Sphere(vec3(0.4, 0.4, 0.4), vec3( 0, 5, -5), 0.1));
	scene.addLight(new Sphere(vec3(0.6, 0.6, 0.8), vec3(-3, 5, -1), 0.1));
	scene.addLight(new Sphere(vec3(1.0, 1.0, 1.0), vec3(-2, 2,  5), 0.1));

	PinHoleCamera camera(vec3(0, 0, 5), vec3(0, 0, -1), vec3(0, 1, 0), radians(60.0f), radio);
	camera.setScene(&scene);

	Mat result(camera.getHeight(width), width, CV_32FC3);
	camera.render(result, 1);
	imshow("Raytracing", result);
	waitKey();

	// vector<int> params;
	// params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	// params.push_back(0);
	// imwrite("Raytracing.png", result, params);

	return 0;
}