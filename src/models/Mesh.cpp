#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.hpp"

bool Mesh::intersect(const vec3 &position, const vec3 &direction, float &t, vec3 &hit_normal, vec3 &hit_surface_color) const {
	return bbox.intersect(position, direction, t, hit_normal, hit_surface_color);
}

void Mesh::loadObj(const char* file_name, const vec3 &surface_color) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(file_name, 0);
	const aiMesh* mesh = scene->mMeshes[0];

	vertices.reserve(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		aiVector3D pos = mesh->mVertices[i];
		vertices.push_back(vec3(pos.x, pos.y, pos.z));
	}

	tris.reserve(mesh->mNumFaces);
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		tris.push_back(Triangle(vertices[mesh->mFaces[i].mIndices[0]], vertices[mesh->mFaces[i].mIndices[1]], vertices[mesh->mFaces[i].mIndices[2]], surface_color));
	}
}

bool Triangle::intersect(const vec3 &position, const vec3 &direction, float &t, vec3 &hit_normal, vec3 &hit_surface_color) const {
	vec3 s = a - position;
	float d12 = determinant(mat3(direction, e1, e2));
	if (abs(d12) < EPSILON)
		return false;

	t = determinant(mat3(s, e1, e2)) / d12;
	if (t < 0)
		return false;

	float beta = determinant(mat3(direction, s, e2)) / d12, gama = determinant(mat3(direction, e1, s)) / d12;
	if (beta < 0 || gama < 0 || beta > 1 || gama > 1 || beta + gama > 1)
		return false;

	hit_normal = normal;
	hit_surface_color = surface_color;
	return true;
}