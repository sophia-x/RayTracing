#ifndef SPHERE_MODEL
#define SPHERE_MODEL

#include "Model.hpp"
#include "Sphere.hpp"
#include "../common.hpp"

class SphereModel : public Model {
protected:
	Sphere __sphere;

public:
	SphereModel(const Material &material): Model(material), __sphere(__hash_code, __material) {}

	SphereModel(const vec3 &center, float radius, const Material &material):
		Model(material), __sphere(center, radius, __hash_code, __material) {}

	~SphereModel() {}

	inline void addPrimitives(vector<Primitive *> &primitives) {
		primitives.push_back(&__sphere);
	}

	void transform(const mat4 &transform_matrix) {
		__sphere.transform(transform_matrix);
	}
};

#endif