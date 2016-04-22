#ifndef MODEL_SET
#define MODEL_SET

#include "basic_model.hpp"
#include "../common.hpp"
#include "../utils/BoundingBox.hpp"

class ModelSet : public BasicModel {
private:
	vector<BasicModel *> models;
	BoundingBox<BasicModel> bbox;

public:
	ModelSet(const vector<BasicModel *> &__models, size_t threshold): BasicModel(Material(), false, vec3(0), vec3(0)), models(__models) {
		bbox = BoundingBox<BasicModel>(models, threshold);
	}

	~ModelSet() {
		for (vector<BasicModel *>::iterator it = models.begin(); it != models.end(); ++it) {
			delete (*it);
		}
	}

	bool intersect(const vec3 &position, const vec3 &direction, float &t, vec3 &hit_normal, vec3 &hit_surface_color, BasicModel const* &hit_model) const;

	inline vec3 getMinPs() const {
		return bbox.getMinPs();
	}

	inline vec3 getMaxPs() const {
		return bbox.getMaxPs();
	}
};

#endif