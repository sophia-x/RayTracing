#ifndef MODEL_SET
#define MODEL_SET

#include "Model.hpp"
#include "../common.hpp"

class ModelSet : public Model {
private:
	vector<Model *> __models;
	vector<mat4> __trans_mats;

public:
	ModelSet(const vector<Model *> &models, const vector<mat4> &trans_mats): Model(Material()), __models(models), __trans_mats(trans_mats) {
		for (int i = 0; i < __models.size(); i++)
			__models[i]->transform(__trans_mats[i]);
	}

	~ModelSet() {
		for (auto it = __models.begin(); it != __models.end(); ++it) {
			delete (*it);
		}
	}

	void transform(const mat4 &transform_matrix) {
		for (int i = 0; i < __models.size(); i ++)
			__models[i]->transform(transform_matrix * __trans_mats[i]);
	}

	inline void addPrimitives(vector<Primitive *> &primitives) {
		for (size_t i = 0; i < __models.size(); i++)
			__models[i]->addPrimitives(primitives);
	}
};

#endif