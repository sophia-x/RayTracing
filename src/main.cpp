#include <opencv2/highgui/highgui.hpp>
#include <chrono>

#include "demo/demo.hpp"

int main() {
	auto begin = chrono::system_clock::now();

	// Mat result = scene_normal();
	// Mat result = scene_area_light();
	// Mat result = scene_depth_camera();
	// Mat result = scene_normal_texture();

	Mat result = scene_transform_demo();

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