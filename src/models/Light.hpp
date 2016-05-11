#ifndef LIGHT
#define LIGHT

#include "Model.hpp"
#include "../common.hpp"

class Scene;

class Light : public Model {
protected:
	vec3 __emission_color;

public:
	Light(const Material &material, const vec3 &emission_color): Model(material), __emission_color(emission_color) {}
	virtual ~Light() {}

	virtual vec3 calcShade(const Scene *scene_ptr, const vec3 &hit_position, float &shade_idx) const = 0;

	virtual void transform(const mat4 &transform_matrix){
		
	}

	inline const vec3 &getEmissionColor() const {
		return __emission_color;
	}
};

#endif