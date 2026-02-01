#pragma once
#include <vector>
#include <mesh.h>
#include <shader.h>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class Model
{
public:
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;
	bool gammaCorrection;

	Model(const std::string &path, bool gamma = true): gammaCorrection(gamma)
	{
		loadModel(path);
	}
	void Draw(Shader& shader);
private:
	void loadModel(const std::string &path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};