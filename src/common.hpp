#ifndef COMMON
#define COMMON

#include <glm/glm.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <limits>

#include <iostream>
#include <glm/gtx/string_cast.hpp>

using namespace glm;
using namespace std;
using namespace cv;

const static float EPSILON = 0.0001;
const static float REFLACT_EPSILON = 0.00001;

const static size_t SAMPLES = 16;
const static size_t SHADE_SIZE = 4;
const static size_t SHADE_SIZE_2 = SHADE_SIZE * SHADE_SIZE;

#endif