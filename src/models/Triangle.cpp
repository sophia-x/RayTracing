#include "Triangle.hpp"

#define INTERSECT() 								\
const vec3 &position = ray.getPosition();			\
const vec3 &direction = ray.getDirection();			\
vec3 pvec = cross(direction, __e2);					\
float det = dot(__e1, pvec);						\
vec3 tvec = position - __a;							\
vec3 qvec = cross(tvec, __e1);						\
float u = dot(tvec, pvec);							\
float v = dot(direction, qvec);						\
if (det > EPSILON) {								\
	if (u < 0.0 || u > det)							\
		return false;								\
	if (v < 0.0 || u + v > det)						\
		return false;								\
} else if (det < -EPSILON) {						\
	if (u > 0.0 || u < det)							\
		return false;								\
	if (v > 0.0 || u + v < det)						\
		return false;								\
} else {											\
	return false;									\
}													\
float inv_det = 1.0 / det;							\
t = dot(__e2, qvec) * inv_det;						\
if(t < 0) return false;								\

bool Triangle::intersect(const Ray &ray, float &t) {
	INTERSECT()

	__u = u * inv_det;
	__v = v * inv_det;
	return true;
}

bool Triangle::intersect_seg(const Ray &ray, float len) const {
	float t;
	INTERSECT()

	return t < len;
}

#define FINDMINMAX(x0,x1,x2,min,max) 					\
min = max = x0;   										\
if (x1 < min) min = x1; 								\
if (x1 > max) max = x1; 								\
if (x2 < min) min = x2; 								\
if (x2 > max) max = x2;

/*======================== X-tests ========================*/
#define AXISTEST_X01(a, b, fa, fb)			   			\
p0 = a * v0[1] - b * v0[2];			       	   			\
p2 = a * v2[1] - b * v2[2];			       	   			\
if (p0 < p2) {											\
	min = p0; max = p2;									\
} else {												\
	min = p2; max = p0;									\
}							 							\
rad = fa * box_half_size[1] + fb * box_half_size[2];   	\
if (min > rad || max < -rad)							\
	return false;

#define AXISTEST_X2(a, b, fa, fb)			   			\
p0 = a * v0[1] - b * v0[2];			           			\
p1 = a * v1[1] - b * v1[2];			       	   			\
if (p0 < p1) {											\
	min = p0; max = p1;									\
} else {												\
	min = p1; max = p0;									\
} 														\
rad = fa * box_half_size[1] + fb * box_half_size[2]; 	\
if (min > rad || max < -rad)							\
	return false;
/*======================== Y-tests ========================*/
#define AXISTEST_Y02(a, b, fa, fb)			   			\
p0 = -a * v0[0] + b * v0[2];				      	   	\
p2 = -a * v2[0] + b * v2[2];	       	       	   		\
if (p0 < p2) {											\
	min = p0; max = p2;									\
} else {												\
	min = p2; max = p0;									\
} 														\
rad = fa * box_half_size[0] + fb * box_half_size[2];   	\
if (min > rad || max < -rad)							\
	return false;										\

#define AXISTEST_Y1(a, b, fa, fb)			   			\
p0 = -a * v0[0] + b * v0[2];		      	   			\
p1 = -a * v1[0] + b * v1[2];	     	       	   		\
if (p0 < p1) {											\
	min = p0; max = p1;									\
} else {												\
	min = p1; max = p0;									\
} 														\
rad = fa * box_half_size[0] + fb * box_half_size[2];   	\
if (min > rad || max < -rad)							\
	return false;
/*======================== Z-tests ========================*/
#define AXISTEST_Z12(a, b, fa, fb)			   			\
p1 = a * v1[0] - b * v1[1];			           			\
p2 = a * v2[0] - b * v2[1];			       	   			\
if (p2 < p1) {											\
	min = p2; max = p1;									\
} else {												\
	min = p1; max = p2;									\
} 														\
rad = fa * box_half_size[0] + fb * box_half_size[1];   	\
if (min > rad || max < -rad) 							\
	return false;										\

#define AXISTEST_Z0(a, b, fa, fb)			   			\
p0 = a * v0[0] - b * v0[1];				   				\
p1 = a * v1[0] - b * v1[1];			           			\
if (p0 < p1) {											\
	min = p0; max = p1;									\
} else {												\
	min = p1; max = p0;									\
} 														\
rad = fa * box_half_size[0] + fb * box_half_size[1];   	\
if (min > rad || max < -rad)							\
	return false;

bool Triangle::intersect(const AABB &box) const {
	float min, max, p0, p1, p2, rad;

	const vec3 &box_center = box.getPos();
	const vec3 &box_half_size = box.getHalfSize();

	vec3 v0 = __a - box_center;
	vec3 v1 = __b - box_center;
	vec3 v2 = __c - box_center;

	vec3 e0 = v1 - v0;
	vec3 e1 = v2 - v1;
	vec3 e2 = v0 - v2;

	float fex = fabsf(e0[0]);
	float fey = fabsf(e0[1]);
	float fez = fabsf(e0[2]);
	AXISTEST_X01(e0[2], e0[1], fez, fey);
	AXISTEST_Y02(e0[2], e0[0], fez, fex);
	AXISTEST_Z12(e0[1], e0[0], fey, fex);

	fex = fabsf(e1[0]);
	fey = fabsf(e1[1]);
	fez = fabsf(e1[2]);
	AXISTEST_X01(e1[2], e1[1], fez, fey);
	AXISTEST_Y02(e1[2], e1[0], fez, fex);
	AXISTEST_Z0(e1[1], e1[0], fey, fex);

	fex = fabsf(e2[0]);
	fey = fabsf(e2[1]);
	fez = fabsf(e2[2]);
	AXISTEST_X2(e2[2], e2[1], fez, fey);
	AXISTEST_Y1(e2[2], e2[0], fez, fex);
	AXISTEST_Z12(e2[1], e2[0], fey, fex);

	FINDMINMAX(v0[0], v1[0], v2[0], min, max);
	if (min > box_half_size[0] || max < -box_half_size[0])
		return false;

	FINDMINMAX(v0[1], v1[1], v2[1], min, max);
	if (min > box_half_size[1] || max < -box_half_size[1])
		return false;

	FINDMINMAX(v0[2], v1[2], v2[2], min, max);
	if (min > box_half_size[2] || max < -box_half_size[2])
		return false;

	vec3 normal = cross(e0, e1);
	if (!__planeBoxOverlap(normal, v0, box_half_size))
		return false;

	return true;
}

inline bool __planeBoxOverlap(const vec3& normal, const vec3 &v0, const vec3 &box_half_size) {
	float v;
	vec3 vmin, vmax;

	for (size_t q = 0; q < 3; q++) {
		v = v0[q];
		if (normal[q] > 0.0f) {
			vmin[q] = -box_half_size[q] - v;
			vmax[q] = box_half_size[q] - v;
		} else {
			vmin[q] = box_half_size[q] - v;
			vmax[q] = -box_half_size[q] - v;
		}
	}

	if (dot(normal, vmin) > 0.0f)
		return false;
	if (dot(normal, vmax) >= 0.0f)
		return true;
	return false;
}