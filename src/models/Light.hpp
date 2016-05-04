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

	virtual float calcShade(const Scene *scene_ptr, const vec3 &hit_position, const vec3 &normal, const vec3 &reflect_ray_dir, const Material &material, const vec3 &surface_color, vec3 &color) const = 0;

	inline const vec3 &getEmissionColor() const {
		return __emission_color;
	}
};

#endif