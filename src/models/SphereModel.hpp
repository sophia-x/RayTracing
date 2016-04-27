#ifndef SPHERE_MODEL
#define SPHERE_MODEL

#include "Model.hpp"
#include "Sphere.hpp"
#include "../common.hpp"

class SphereModel : public Model {
protected:
	Sphere __sphere;

public:
	SphereModel(const vec3 &center, float radius, const vec3 &surface_color, const Material &material):
		Model(material), __sphere(center, radius, __hash_code, surface_color, __material) {}

	virtual ~SphereModel() {}

	inline virtual void addPrimitives(vector<const Primitive *> &primitives) const {
		primitives.push_back(&__sphere);
	}
};

#endif