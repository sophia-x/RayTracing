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

public:
	Model(const Material &material): __material(material), __hash_code(MODEL_NUM ++) {}

	virtual ~Model() {}

	virtual void addPrimitives(vector<Primitive *> &primitives) = 0;

	inline unsigned long getHashCode() const {
		return __hash_code;
	}
};
#endif