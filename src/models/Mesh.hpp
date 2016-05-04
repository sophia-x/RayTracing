#ifndef MESH
#define MESH

#include "Model.hpp"
#include "Triangle.hpp"
#include "../common.hpp"

class Mesh : public Model {
private:
	vector<Triangle> __tris;
	vector<vec3> __vertices;

public:
	Mesh(const char *file_name, const vec3 &surface_color, const Material &material):
		Model(material, surface_color) {
		loadObj(file_name, __surface_color);
	}

	Mesh(const vector<vec3> &vertices, const vector<int> &tri_idx, const vec3 &surface_color, const Material &material):
		Model(material, surface_color), __vertices(vertices) {

		size_t size = tri_idx.size() / 3;
		__tris.reserve(size);

		for (size_t i = 0; i < size; i++) {
			__tris.push_back(Triangle(__vertices[tri_idx[3 * i]], __vertices[tri_idx[3 * i + 1]], __vertices[tri_idx[3 * i + 2]], __hash_code, __surface_color, __material));
		}
	}

	inline void addPrimitives(vector<const Primitive *> &primitives) const {
		for (size_t i = 0; i < __tris.size(); i++)
			primitives.push_back(&(__tris[i]));
	}
private:
	void loadObj(const char* file_name, const vec3 &surface_color);
};

#endif