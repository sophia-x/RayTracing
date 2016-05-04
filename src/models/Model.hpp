#ifndef MODEL
#define MODEL

#include "Material.hpp"
#include "Primitive.hpp"
#include "../common.hpp"

class Model {
protected:
	static int MODEL_NUM;

	Material __material;
	unsigned long __hash_code;
	vec3 __surface_color;

public:
	Model(const Material &material, const vec3 &surface_color): __material(material), __hash_code(MODEL_NUM ++), __surface_color(surface_color) {}

	virtual ~Model() {}

	virtual void addPrimitives(vector<const Primitive *> &primitives) const = 0;

	inline unsigned long getHashCode() const {
		return __hash_code;
	}
};
#endif