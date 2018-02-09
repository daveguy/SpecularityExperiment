#include "ObjectLoader.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"


bool ObjectLoader::LoadObjectFile(
	const char * path, 
	std::vector<unsigned short>& indices,
	std::vector<glm::vec3>& vertices,
	std::vector<glm::vec2>& uvs,
	std::vector<glm::vec3>& normals)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_JoinIdenticalVertices);

	if (!scene)
	{
		fprintf(stderr, importer.GetErrorString());
		getchar();
		return false;
	}

	//get the first mesh
	const aiMesh* mesh = scene->mMeshes[0];
	//fill the vertices
	vertices.reserve(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		aiVector3D pos = mesh->mVertices[i];
		vertices.push_back(glm::vec3( pos.x, pos.y, pos.z ));
	}

	//fill UV's, I don't use them but if you add textures
	/*uvs.reserve(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		aiVector3D uv = mesh->mTextureCoords[0][i];
		uvs.push_back({ uv.x, uv.y });
	}*/

	normals.reserve(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		aiVector3D norm = mesh->mNormals[i];
		normals.push_back(glm::vec3( norm.x, norm.y, norm.z ));
	}

	//fill the indices
	indices.reserve(mesh->mNumFaces);
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		indices.push_back(mesh->mFaces[i].mIndices[0]);
		indices.push_back(mesh->mFaces[i].mIndices[1]);
		indices.push_back(mesh->mFaces[i].mIndices[2]);
	}

	// The "scene" pointer will be deleted automatically by "importer"
	return true;
}
