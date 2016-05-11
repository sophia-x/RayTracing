#ifndef MESH
#define MESH

#include "Model.hpp"
#include "Triangle.hpp"
#include "../common.hpp"

class Mesh : public Model {
private:
	vector<Triangle> __tris;
	vector<vec3> __vertices;
	vector<vec3> __model_verticles;
	vector<vec2> __uvs;

public:
	Mesh(const char *file_name, const Material &material):
		Model(material) {
		loadObj(file_name);
		buildModel();
	}

	Mesh(const vector<vec3> &vertices, const vector<int> &tri_idx, const Material &material):
		Model(material), __vertices(vertices) {

		size_t size = tri_idx.size() / 3;
		__tris.reserve(size);

		for (size_t i = 0; i < size; i++) {
			__tris.push_back(Triangle(__vertices[tri_idx[3 * i]], __vertices[tri_idx[3 * i + 1]], __vertices[tri_idx[3 * i + 2]], __hash_code, __material));
		}
		buildModel();
	}

	Mesh(const vector<vec3> &vertices, const vector<vec2> &uvs, const vector<int> &tri_idx, const Material &material):
		Model(material), __vertices(vertices), __uvs(uvs) {

		size_t size = tri_idx.size() / 3;
		__tris.reserve(size);

		for (size_t i = 0; i < size; i++) {
			__tris.push_back(Triangle(__vertices[tri_idx[3 * i]], __vertices[tri_idx[3 * i + 1]], __vertices[tri_idx[3 * i + 2]], __uvs[tri_idx[3 * i]], __uvs[tri_idx[3 * i + 1]], __uvs[tri_idx[3 * i + 2]], __hash_code, __material));
		}
		buildModel();
	}

	void transform(const mat4 &transform_matrix) {
		for (int i = 0; i < __vertices.size(); i ++) {
			__vertices[i] = vec3(transform_matrix * vec4(__model_verticles[i], 1));
		}

		for (auto tri = __tris.begin(); tri != __tris.end(); tri++)
			tri->update();
	}

	inline void addPrimitives(vector<Primitive *> &primitives) {
		for (size_t i = 0; i < __tris.size(); i++)
			primitives.push_back(&(__tris[i]));
	}
private:
	void loadObj(const char* file_name);
	void buildModel() {
		vec3 mean(0), max_p(numeric_limits<float>::min()), min_p(numeric_limits<float>::max());
		for (int i = 0; i < __vertices.size(); i++) {
			mean += __vertices[i];
			max_p = max(max_p, __vertices[i]);
			min_p = min(min_p, __vertices[i]);
		}
		mean /= float(__vertices.size());
		vec3 m = max(abs(max_p - mean), abs(min_p - mean));

		__model_verticles.reserve(__vertices.size());
		for (int i = 0; i < __vertices.size(); i ++) {
			vec3 p = __vertices[i] - mean;
			for (int j = 0; j < 3; j ++) {
				if (m[j] >= EPSILON) {
					p[j] /= m[j];
				}
			}
			__model_verticles.push_back(p);
		}
	}
};

#endif