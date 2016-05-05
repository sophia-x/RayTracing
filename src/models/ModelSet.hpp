#ifndef MODEL_SET
#define MODEL_SET

#include "Model.hpp"
#include "../common.hpp"

class ModelSet : public Model {
private:
	vector<Model *> __models;

public:
	ModelSet(const vector<Model *> &models): Model(Material()), __models(models) {}

	~ModelSet() {
		for (auto it = __models.begin(); it != __models.end(); ++it) {
			delete (*it);
		}
	}

	inline void addPrimitives(vector<Primitive *> &primitives) {
		for (size_t i = 0; i < __models.size(); i++)
			__models[i]->addPrimitives(primitives);
	}
};

#endif