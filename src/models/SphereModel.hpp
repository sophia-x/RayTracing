#ifndef SPHERE_MODEL
#define SPHERE_MODEL

#include "Model.hpp"
#include "Sphere.hpp"
#include "../common.hpp"

class SphereModel : public Model {
protected:
	Sphere __sphere;

public:
	SphereModel(const vec3 &center, float radius, const Material &material):
		Model(material), __sphere(center, radius, __hash_code, __material) {}

	virtual ~SphereModel() {}

	inline virtual void addPrimitives(vector<Primitive *> &primitives){
		primitives.push_back(&__sphere);
	}
};

#endif