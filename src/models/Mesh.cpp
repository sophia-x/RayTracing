#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.hpp"

void Mesh::loadObj(const char* file_name) {
	cout << "Load OBJ from " << file_name << endl;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(file_name, 0);
	const aiMesh* mesh = scene->mMeshes[0];

	__vertices.reserve(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		aiVector3D pos = mesh->mVertices[i];
		__vertices.push_back(vec3(pos.x, pos.y, pos.z));
	}

	if (__material.hasTexture()) {
		__uvs.reserve(mesh->mNumVertices);
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			aiVector3D UVW = mesh->mTextureCoords[0][i];
			__uvs.push_back(vec2(UVW.x, UVW.y));
		}
	}

	__tris.reserve(mesh->mNumFaces);

	cout << "\t" << mesh->mNumFaces << " triangle in this obj" << endl;

	if (__material.hasTexture()) {
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			__tris.push_back(Triangle(__vertices[mesh->mFaces[i].mIndices[0]], __vertices[mesh->mFaces[i].mIndices[1]], __vertices[mesh->mFaces[i].mIndices[2]], __uvs[mesh->mFaces[i].mIndices[0]], __uvs[mesh->mFaces[i].mIndices[1]], __uvs[mesh->mFaces[i].mIndices[2]], __hash_code, __material));
		}
	} else {
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			__tris.push_back(Triangle(__vertices[mesh->mFaces[i].mIndices[0]], __vertices[mesh->mFaces[i].mIndices[1]], __vertices[mesh->mFaces[i].mIndices[2]], __hash_code, __material));
		}
	}
}