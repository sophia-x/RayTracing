#ifndef MESH
#define MESH

#include "basic_model.hpp"
#include "../common.hpp"
#include "../utils/BoundingBox.hpp"

class Triangle {
private:
	const vec3 &a, &b, &c;
	vec3 e1, e2;
	vec3 normal;
	const vec3 &surface_color;

public:
	Triangle(const vec3 &a, const vec3 &b, const vec3 &c, const vec3 &surface_color): a(a), b(b), c(c), surface_color(surface_color),
		normal(normalize(cross(a - b, b - c))), e1(a - b), e2(a - c) {}

	bool intersect(const vec3 &position, const vec3 &direction, float &t, vec3 &hit_normal, vec3 &hit_surface_color, BasicModel const* &hit_model) const;

	bool intersectBox(const Box &box) const;

	inline vec3 getMinPs() const {
		return glm::min(a, glm::min(b, c));
	}

	inline vec3 getMaxPs() const {
		return glm::max(a, glm::max(b, c));
	}
};

class Mesh : public BasicModel {
private:
	vector<Triangle> tris;
	vector<vec3> vertices;
	BoundingBox<Triangle> bbox;

public:
	Mesh(const char *file_name, const vec3 &surface_color, const Material &material):
		BasicModel(material, false, vec3(0.0f), vec3(0)) {
		loadObj(file_name, surface_color);

		size_t size = tris.size();
		vector<Triangle *> tri_ptrs(size);
		for (size_t i = 0; i < size; i++)
			tri_ptrs[i] = &tris[i];
		bbox = BoundingBox<Triangle>(tri_ptrs);
	}

	Mesh(const vector<vec3> &__vertices, const vector<int> &tri_idx, const vec3 &surface_color, const Material &material):
		BasicModel(material, false, vec3(0), vec3(0)), vertices(__vertices) {

		size_t size = tri_idx.size() / 3;
		tris.reserve(size);
		vector<Triangle *> tri_ptrs(size);

		for (size_t i = 0; i < size; i++) {
			tris.push_back(Triangle(vertices[tri_idx[3 * i]], vertices[tri_idx[3 * i + 1]], vertices[tri_idx[3 * i + 2]], surface_color));
			tri_ptrs[i] = &tris[i];
		}
		bbox = BoundingBox<Triangle>(tri_ptrs);
	}

	bool intersect(const vec3 &position, const vec3 &direction, float &t, vec3 &hit_normal, vec3 &hit_surface_color, BasicModel const* &hit_model) const;

	inline bool intersectBox(const Box &box) const {
		return bbox.intersectBox(box);
	}

	inline vec3 getMinPs() const {
		return bbox.getMinPs();
	}

	inline vec3 getMaxPs() const {
		return bbox.getMaxPs();
	}
private:
	void loadObj(const char* file_name, const vec3 &surface_color);
};

#endif